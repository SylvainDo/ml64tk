#pragma once

#include <memory>
#include <string>

#include <gtk/gtk.h>

namespace gui {
inline namespace util {

#define GObjectAutoPtr(T, name, value) std::unique_ptr<T, decltype(&g_object_unref)> name{ value, g_object_unref }
#define GStringAutoPtr(name, value) std::unique_ptr<gchar, decltype(&g_free)> name{ value, g_free }

inline std::string stringFromGString(gchar* str) {
    const GStringAutoPtr(str_, str);
    return str;
}

inline void gtkWindowWidgetDestroy(GtkWidget* widget) { gtk_window_destroy(GTK_WINDOW(widget)); }
using GtkWindowAutoPtr_t = std::unique_ptr<GtkWidget, decltype(&gtkWindowWidgetDestroy)>;
#define GtkWindowAutoPtr(name, value) GtkWindowAutoPtr_t name{ value, gtkWindowWidgetDestroy }

}
}
