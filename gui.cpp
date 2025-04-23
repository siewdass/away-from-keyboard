#include <gtk/gtk.h>
#include <filesystem>
#include <string>
#include <vector>
#include <sys/stat.h>

namespace fs = std::filesystem;
using namespace std;

static void on_dropdown_changed(GtkComboBox *widget, gpointer data) {
  gint active = gtk_combo_box_get_active(widget);
  g_print("Selected item: %d\n", active);
}

static void on_window_closed(GtkWidget *widget, gpointer data) {
  gtk_main_quit();
}

vector<string> get_files(const string& directory) {
  vector<string> files;
  
  try {
    for (const auto& entry : fs::directory_iterator(directory)) {
      if (entry.is_regular_file() && entry.path().extension() == ".json") {
        files.push_back(entry.path().stem().string());
      }
    }
  } catch (const fs::filesystem_error& e) {
    g_printerr("Error accessing directory: %s\n", e.what());
  }
  
  return files;
}

static void on_start_clicked(GtkWidget *widget, gpointer data) {
  g_print("Start button clicked\n");
}

static void on_stop_clicked(GtkWidget *widget, gpointer data) {
  g_print("Stop button clicked\n");
}

static void on_restart_clicked(GtkWidget *widget, gpointer data) {
  g_print("Restart button clicked\n");
}

int main(int argc, char *argv[]) {
  GtkWidget *window, *box, *button_box, *button_open;
  GtkWidget *dropdown_char, *dropdown_waypoint;
  GtkWidget *label_char, *label_waypoint, *label_bot, *label_client;
  GtkWidget *start_btn, *stop_btn, *restart_btn, *viewer_btn, *open_btn;
  
  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "AFK");
  gtk_window_set_default_size(GTK_WINDOW(window), 350, 400);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  g_signal_connect(window, "destroy", G_CALLBACK(on_window_closed), NULL);

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(window), box);

  {
    label_char = gtk_label_new("Session:");
    gtk_label_set_xalign(GTK_LABEL(label_char), 0.0);
    gtk_widget_set_margin_start(label_char, 3);
    gtk_box_pack_start(GTK_BOX(box), label_char, FALSE, FALSE, 0);
    dropdown_char = gtk_combo_box_text_new();
    for (int i = 1; i <= 4; i++) {
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropdown_char), to_string(i).c_str());
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropdown_char), 0);
    gtk_box_pack_start(GTK_BOX(box), dropdown_char, FALSE, FALSE, 0);
    g_signal_connect(dropdown_char, "changed", G_CALLBACK(on_dropdown_changed), NULL);
  }

  {
    label_client = gtk_label_new("Tibia:");
    gtk_label_set_xalign(GTK_LABEL(label_client), 0.0);
    gtk_widget_set_margin_start(label_client, 0);
    gtk_box_pack_start(GTK_BOX(box), label_client, FALSE, FALSE, 0);

    button_open = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_set_homogeneous(GTK_BOX(button_open), TRUE);

    viewer_btn = gtk_button_new_with_label("Viewer");
    open_btn = gtk_button_new_with_label("Client");
    
    g_signal_connect(viewer_btn, "clicked", G_CALLBACK(on_start_clicked), NULL);
    g_signal_connect(open_btn, "clicked", G_CALLBACK(on_stop_clicked), NULL);
    
    gtk_box_pack_start(GTK_BOX(button_open), viewer_btn, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_open), open_btn, TRUE, TRUE, 0);
    
    gtk_box_pack_start(GTK_BOX(box), button_open, FALSE, FALSE, 5);
  }

  {
    label_bot = gtk_label_new("Bot:");
    gtk_label_set_xalign(GTK_LABEL(label_bot), 0.0);
    gtk_widget_set_margin_start(label_bot, 0);
    gtk_box_pack_start(GTK_BOX(box), label_bot, FALSE, FALSE, 0);

    button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_set_homogeneous(GTK_BOX(button_box), TRUE);

    start_btn = gtk_button_new_with_label("Start");
    stop_btn = gtk_button_new_with_label("Stop");
    restart_btn = gtk_button_new_with_label("Restart");
    
    g_signal_connect(start_btn, "clicked", G_CALLBACK(on_start_clicked), NULL);
    g_signal_connect(stop_btn, "clicked", G_CALLBACK(on_stop_clicked), NULL);
    g_signal_connect(restart_btn, "clicked", G_CALLBACK(on_restart_clicked), NULL);
    
    gtk_box_pack_start(GTK_BOX(button_box), start_btn, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), restart_btn, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), stop_btn, TRUE, TRUE, 0);
    
    gtk_box_pack_start(GTK_BOX(box), button_box, FALSE, FALSE, 5);
  }

  {
    label_char = gtk_label_new("Character:");
    gtk_label_set_xalign(GTK_LABEL(label_char), 0.0);
    gtk_widget_set_margin_start(label_char, 3);
    gtk_box_pack_start(GTK_BOX(box), label_char, FALSE, FALSE, 0);
    dropdown_char = gtk_combo_box_text_new();
    for (const auto& file : get_files("/opt/afk/characters")) {
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropdown_char), file.c_str());
    }
    gtk_box_pack_start(GTK_BOX(box), dropdown_char, FALSE, FALSE, 0);
    g_signal_connect(dropdown_char, "changed", G_CALLBACK(on_dropdown_changed), NULL);
  }

  {
    label_waypoint = gtk_label_new("Waypoint:");
    gtk_label_set_xalign(GTK_LABEL(label_waypoint), 0.0);
    gtk_widget_set_margin_start(label_waypoint, 3);
    gtk_box_pack_start(GTK_BOX(box), label_waypoint, FALSE, FALSE, 0);
    dropdown_waypoint = gtk_combo_box_text_new();
    for (const auto& file : get_files("/opt/afk/waypoints")) {
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropdown_waypoint), file.c_str());
    }
    gtk_box_pack_start(GTK_BOX(box), dropdown_waypoint, FALSE, FALSE, 0);
    g_signal_connect(dropdown_waypoint, "changed", G_CALLBACK(on_dropdown_changed), NULL);
  }

  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_widget_show_all(window);
  gtk_main();
  
  return 0;
}