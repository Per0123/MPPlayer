#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

static void open_file(GtkWidget *widget, WebKitWebView *web_view) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Open Media",
        GTK_WINDOW(gtk_widget_get_toplevel(widget)),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Cancel", GTK_RESPONSE_CANCEL,
        "_Open", GTK_RESPONSE_ACCEPT,
        NULL
    );

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        char html[4096];
        snprintf(html, sizeof(html),
                 "<html><body style='margin:0; background:black;'>"
                 "<video src='file://%s' controls autoplay style='width:100%%; height:100%%;'></video>"
                 "</body></html>",
                 filename);

        webkit_web_view_load_html(web_view, html, "file:///");
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK WebKit Media Player");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 5);

    GtkWidget *btn_open = gtk_button_new_with_label("Open File");
    gtk_box_pack_start(GTK_BOX(toolbar), btn_open, FALSE, FALSE, 5);

    WebKitWebView *web_view = WEBKIT_WEB_VIEW(webkit_web_view_new());
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(web_view), TRUE, TRUE, 0);

    g_signal_connect(btn_open, "clicked", G_CALLBACK(open_file), web_view);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

// gcc main.cpp -o MPPlayer `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0`
