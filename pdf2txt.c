#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <poppler/glib/poppler.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s [--stdout] <path_to_pdf>\n", argv[0]);
        return 1;
    }

    gchar *pdf_path = NULL;
    gboolean stdout_flag = FALSE;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--stdout") == 0) {
            stdout_flag = TRUE;
        } else {
            pdf_path = argv[i];
        }
    }

    if (pdf_path == NULL) {
        fprintf(stderr, "Error: PDF file path is missing.\n");
        return 1;
    }

    GError *error = NULL;
    char absolute_path[PATH_MAX];
    realpath(pdf_path, absolute_path);
    gchar *pdf_uri = g_filename_to_uri(absolute_path, NULL, &error);

    if (pdf_uri == NULL) {
        fprintf(stderr, "Error converting file path to URI: %s\n", error->message);
        g_error_free(error);
        return 1;
    }

    PopplerDocument *doc = poppler_document_new_from_file(pdf_uri, NULL, &error);
    g_free(pdf_uri);

    if (doc == NULL) {
        fprintf(stderr, "Error opening PDF file: %s\n", error->message);
        g_error_free(error);
        return 1;
    }

    GString *pdf_text = g_string_new("");
    int num_pages = poppler_document_get_n_pages(doc);

    gchar **page_texts = g_new(gchar *, num_pages);

    #pragma omp parallel for
    for (int i = 0; i < num_pages; i++) {
        PopplerPage *page = poppler_document_get_page(doc, i);
        if (page) {
            gchar *text = poppler_page_get_text(page);
            page_texts[i] = text;
            g_object_unref(page);
        }
    }

    for (int i = 0; i < num_pages; i++) {
        g_string_append_printf(pdf_text, "%s\n", page_texts[i]);
        g_free(page_texts[i]);
    }
    g_free(page_texts);

    if (stdout_flag) {
        printf("%s\n", pdf_text->str);
    } else {
        char *base_filename = g_path_get_basename(pdf_path);
        char *filename_no_ext = g_strndup(base_filename, strlen(base_filename) - 4);
        char *filename = g_strdup_printf("%s.txt", filename_no_ext);
        FILE *fp = fopen(filename, "w");
        fprintf(fp, "%s\n", pdf_text->str);
        fclose(fp);
        g_free(base_filename);
        g_free(filename_no_ext);
        g_free(filename);
    }

    g_string_free(pdf_text, TRUE);
    g_object_unref(doc);

    return 0;
}
