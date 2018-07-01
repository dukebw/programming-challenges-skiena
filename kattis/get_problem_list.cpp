#include "curl/curl.h"
#include "gumbo.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stack>

struct raw_html {
        size_t max_len;
        size_t current_len;
        char *data;
};

static size_t
write_raw_html(const void *ptr, size_t size, size_t nmemb, struct raw_html *raw)
{
        size_t chunk_size = size*nmemb;
        assert(chunk_size <= (raw->max_len - raw->current_len));

        memcpy(raw->data + raw->current_len, ptr, chunk_size);
        raw->current_len += chunk_size;

        return chunk_size;
}

/**
 * push_children() - Push all of node's children onto prev_nodes.
 */
static void
push_children(std::stack<GumboNode *> *prev_nodes, GumboNode *node)
{
        GumboVector *children = &node->v.element.children;
        for (uint32_t i = 0;
             i < children->length;
             ++i) {
                prev_nodes->push(static_cast<GumboNode *>(children->data[i]));
        }
}

int main(void)
{
        struct raw_html raw;
        raw.current_len = 0;
        raw.max_len = 1024*1024;
        raw.data = (char *)malloc(raw.max_len);
        assert(raw.data != NULL);

        CURLcode curl_sts = curl_global_init(CURL_GLOBAL_DEFAULT);
        assert(curl_sts == CURLE_OK);

        CURL *curl = curl_easy_init();
        assert(curl != NULL);

        curl_sts = curl_easy_setopt(curl,
                                    CURLOPT_WRITEFUNCTION,
                                    write_raw_html);
        assert(curl_sts == CURLE_OK);

        curl_sts = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &raw);
        assert(curl_sts == CURLE_OK);

        /* HTTP GET the page HTML. */
        constexpr char base_url[] = "https://open.kattis.com/problems?page=";
        constexpr size_t base_url_len = sizeof(base_url) - 1;
        char url[sizeof(base_url) + 2];
        memcpy(url, base_url, sizeof(base_url));

        for (uint32_t page_i = 0;
             ;
             ++page_i) {
                raw.current_len = 0;

                int32_t num_printed = snprintf(url + base_url_len, 3, "%u", page_i);
                url[sizeof(base_url) + num_printed] = '\0';

                curl_sts = curl_easy_setopt(curl, CURLOPT_URL, url);
                assert(curl_sts == CURLE_OK);

                curl_sts = curl_easy_perform(curl);
                assert(curl_sts == CURLE_OK);

                /* Find all the hyperrefs. */
                GumboOutput *out = gumbo_parse(raw.data);
                assert(out != NULL);

                std::stack<GumboNode *> prev_nodes;
                prev_nodes.push(out->root);

                for (;;) {
                        GumboNode *node = prev_nodes.top();
                        prev_nodes.pop();

                        if (node->type != GUMBO_NODE_ELEMENT)
                                continue;

                        if (node->v.element.tag == GUMBO_TAG_TBODY) {
                                GumboVector *children = &node->v.element.children;
                                if (children->length == 1) {
                                        GumboNode *single_child =
                                                static_cast<GumboNode *>(children->data[0]);
                                        if ((single_child->type != GUMBO_NODE_ELEMENT) &&
                                            (single_child->type != GUMBO_NODE_DOCUMENT))
                                                goto finish;
                                }

                                prev_nodes = std::stack<GumboNode *>{};
                                prev_nodes.push(node);
                                break;
                        }

                        push_children(&prev_nodes, node);
                }

                for (;;) {
                        if (prev_nodes.empty())
                                break;

                        GumboNode *node = prev_nodes.top();
                        prev_nodes.pop();

                        if (node->type != GUMBO_NODE_ELEMENT)
                                continue;

                        if (node->v.element.tag == GUMBO_TAG_TD) {
                                GumboAttribute *cls = gumbo_get_attribute(&node->v.element.attributes,
                                                                          "class");
                                constexpr char name_col[] = "name_column";
                                if ((cls != NULL) &&
                                    (memcmp(cls->value, name_col, sizeof(name_col) - 1) == 0)) {
                                        GumboVector *children = &node->v.element.children;
                                        GumboNode *href_node =
                                                static_cast<GumboNode *>(children->data[0]);

                                        GumboAttribute *href = gumbo_get_attribute(&href_node->v.element.attributes,
                                                                                   "href");
                                        constexpr char problems[] = "/problems/";
                                        constexpr size_t prob_len = sizeof(problems) - 1;
                                        if ((href != NULL) &&
                                            (memcmp(href->value, problems, prob_len) == 0)) {
                                                printf("%s\n", href->value + prob_len);
                                        }
                                }
                        }

                        push_children(&prev_nodes, node);
                }

                gumbo_destroy_output(&kGumboDefaultOptions, out);
        }

finish:
        return EXIT_SUCCESS;
}
