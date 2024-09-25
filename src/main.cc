#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

using namespace std;

class WebViewApp {
public:
    WebViewApp() {
        app = gtk_application_new("dev.thoq.Purrooser", G_APPLICATION_FLAGS_NONE);
        g_signal_connect(app, "activate", G_CALLBACK(activate), this);
    }

    void run(int argc, char **argv) {
        g_application_run(G_APPLICATION(app), argc, argv);
        g_object_unref(app);
    }

private:
    GtkApplication *app;
    GtkToolItem *reload_button;
    GtkToolItem *stop_button;
    GtkWidget *webview;

    static void activate(GtkApplication *app, gpointer user_data) {
        WebViewApp *self = static_cast<WebViewApp*>(user_data);
        self->create_window();
    }

    void create_window() {
        GtkWidget *window = gtk_application_window_new(app);
        gtk_window_set_title(GTK_WINDOW(window), "Purrooser");
        gtk_window_set_default_size(GTK_WINDOW(window), 1600, 900);

        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add(GTK_CONTAINER(window), box);

        GtkWidget *toolbar = gtk_toolbar_new();
        gtk_box_pack_start(GTK_BOX(box), toolbar, FALSE, FALSE, 0);

        // Back button
        GtkToolItem *back_button = gtk_tool_button_new(NULL, "<");
        g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_button_clicked), this);
        gtk_toolbar_insert(GTK_TOOLBAR(toolbar), back_button, -1);

        // Forward button
        GtkToolItem *forward_button = gtk_tool_button_new(NULL, ">");
        g_signal_connect(forward_button, "clicked", G_CALLBACK(on_forward_button_clicked), this);
        gtk_toolbar_insert(GTK_TOOLBAR(toolbar), forward_button, -1);

        // Reload button
        reload_button = GTK_TOOL_ITEM(gtk_tool_button_new(gtk_image_new_from_icon_name("view-refresh", GTK_ICON_SIZE_SMALL_TOOLBAR), "Reload"));
        g_signal_connect(reload_button, "clicked", G_CALLBACK(on_reload_button_clicked), this);
        gtk_toolbar_insert(GTK_TOOLBAR(toolbar), reload_button, -1);
        gtk_widget_set_visible(GTK_WIDGET(reload_button), TRUE);

        // Stop button
        stop_button = GTK_TOOL_ITEM(gtk_tool_button_new(gtk_image_new_from_icon_name("process-stop", GTK_ICON_SIZE_SMALL_TOOLBAR), "Stop"));
        g_signal_connect(stop_button, "clicked", G_CALLBACK(on_stop_button_clicked), this);
        gtk_toolbar_insert(GTK_TOOLBAR(toolbar), stop_button, -1);
        gtk_widget_set_visible(GTK_WIDGET(stop_button), FALSE);

        // Home button
        GtkToolItem *home_button = gtk_tool_button_new(NULL, "Home");
        g_signal_connect(home_button, "clicked", G_CALLBACK(on_home_button_clicked), this);
        gtk_toolbar_insert(GTK_TOOLBAR(toolbar), home_button, -1);

        webview = webkit_web_view_new();
        gtk_widget_set_vexpand(webview, TRUE);
        gtk_widget_set_hexpand(webview, TRUE);
        gtk_box_pack_start(GTK_BOX(box), webview, TRUE, TRUE, 0);

        const char* server_url = "http://localhost:54654/index.html";
        webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), server_url);

        g_signal_connect(webview, "load-changed", G_CALLBACK(on_load_changed), this);

        const char* css = R"(
            toolbar {
                background-color: #111;
            }
            toolbar .tool-item {
                color: white;
            }
            toolbar .tool-item:hover {
                background-color: #333;
            }
        )";
        GtkCssProvider *css_provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(css_provider, css, -1, NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

        gtk_widget_show_all(window);
    }

    static void on_load_changed(WebKitWebView *webview, WebKitLoadEvent load_event, gpointer user_data) {
        WebViewApp *self = static_cast<WebViewApp*>(user_data);
        switch (load_event) {
            case WEBKIT_LOAD_STARTED:
                gtk_widget_set_visible(GTK_WIDGET(self->reload_button), FALSE);
                gtk_widget_set_visible(GTK_WIDGET(self->stop_button), TRUE);
                break;
            case WEBKIT_LOAD_COMMITTED:
                gtk_widget_set_visible(GTK_WIDGET(self->reload_button), FALSE);
                gtk_widget_set_visible(GTK_WIDGET(self->stop_button), TRUE);
                break;
            case WEBKIT_LOAD_FINISHED:
                gtk_widget_set_visible(GTK_WIDGET(self->reload_button), TRUE);
                gtk_widget_set_visible(GTK_WIDGET(self->stop_button), FALSE);
                break;
            default:
                break;
        }
    }

    static void on_back_button_clicked(GtkToolButton *button, gpointer user_data) {
        WebViewApp *self = static_cast<WebViewApp*>(user_data);
        webkit_web_view_go_back(WEBKIT_WEB_VIEW(self->webview));
    }

    static void on_forward_button_clicked(GtkToolButton *button, gpointer user_data) {
        WebViewApp *self = static_cast<WebViewApp*>(user_data);
        webkit_web_view_go_forward(WEBKIT_WEB_VIEW(self->webview));
    }

    static void on_reload_button_clicked(GtkToolButton *button, gpointer user_data) {
        WebViewApp *self = static_cast<WebViewApp*>(user_data);
        webkit_web_view_reload(WEBKIT_WEB_VIEW(self->webview));
    }

    static void on_stop_button_clicked(GtkToolButton *button, gpointer user_data) {
        WebViewApp *self = static_cast<WebViewApp*>(user_data);
        webkit_web_view_stop_loading(WEBKIT_WEB_VIEW(self->webview));
    }

    static void on_home_button_clicked(GtkToolButton *button, gpointer user_data) {
        WebViewApp *self = static_cast<WebViewApp*>(user_data);
        const char* home_url = "http://localhost:54654/index.html";
        webkit_web_view_load_uri(WEBKIT_WEB_VIEW(self->webview), home_url);
    }
};

int main(int argc, char **argv) {
    WebViewApp app;
    app.run(argc, argv);
    return 0;
}