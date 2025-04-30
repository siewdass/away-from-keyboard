#include <gtk/gtk.h>
#include <filesystem>
#include <string>
#include <vector>
#include <sys/stat.h>

vector<vector<string>> data = {{"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""}};

vector<string> getJson(string directory) {
  vector<string> files;
  for (const auto& entry : std::filesystem::directory_iterator(directory)) {
      files.push_back(entry.path().stem().string());
  }
  return files;
}

vector<string> characters = getJson("/opt/afk/characters");
vector<string> waypoints = getJson("/opt/afk/waypoints");

void open_worker(GtkWidget *widget, gpointer data) {
  int id = GPOINTER_TO_INT(data);
  if (!filesystem::exists(string("/tmp/.X11-unix/X" + to_string(id)).c_str())) { 
    system(string("/opt/afk/worker.sh " + to_string(id)).c_str());
  }
  if (filesystem::exists(string("/tmp/.X11-unix/X" + to_string(id)).c_str())) { 
    system(string("/opt/afk/viewer.sh " + to_string(id)).c_str());
  }
}

void close_worker(GtkWidget *widget, gpointer data) {
  int id = GPOINTER_TO_INT(data);
  system(string("pkill -f 'Xvfb.*:" + to_string(id) + "'").c_str());
}

void on_character_changed(GtkComboBox *widget, gpointer data) {
  int id = GPOINTER_TO_INT(data);
  int active = gtk_combo_box_get_active(widget);
  ::data[id][0] = characters[active];
}

void on_waypoint_changed(GtkComboBox *widget, gpointer data) {
  int id = GPOINTER_TO_INT(data);
  int active = gtk_combo_box_get_active(widget);
  ::data[id][1] = waypoints[active];
}

void on_window_closed(GtkWidget *widget, gpointer data) {
  gtk_main_quit();
}

void on_start_clicked(GtkWidget *widget, gpointer mydata) {
  int id = GPOINTER_TO_INT(mydata);
  if (!::data[id][0].empty() && !::data[id][1].empty()) {
    execute(id, ::data[id][0] + ".json ", ::data[id][1] + ".json");
  } else {
    g_print("Empty character or waypoint name!\n");
  }
}

void on_stop_clicked(GtkWidget *widget, gpointer data) {
  g_print("Stop button clicked\n");
}

GtkWidget* create_page(int id) {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 7);

  {
    GtkWidget *label_worker = gtk_label_new("Client:");
    gtk_label_set_xalign(GTK_LABEL(label_worker), 0.0);
    gtk_widget_set_margin_start(label_worker, 0);
    gtk_box_pack_start(GTK_BOX(box), label_worker, FALSE, FALSE, 0);

    GtkWidget *worker_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_set_homogeneous(GTK_BOX(worker_box), TRUE);

    GtkWidget *open_btn = gtk_button_new_with_label("Open");
    GtkWidget *close_btn = gtk_button_new_with_label("Close");
    
    g_signal_connect(open_btn, "clicked", G_CALLBACK(open_worker), GINT_TO_POINTER(id));
    g_signal_connect(close_btn, "clicked", G_CALLBACK(close_worker), GINT_TO_POINTER(id));
    
    gtk_box_pack_start(GTK_BOX(worker_box), open_btn, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(worker_box), close_btn, TRUE, TRUE, 0);
    
    gtk_box_pack_start(GTK_BOX(box), worker_box, FALSE, FALSE, 5);
  }

  {
    GtkWidget *label_bot = gtk_label_new("Bot:");
    gtk_label_set_xalign(GTK_LABEL(label_bot), 0.0);
    gtk_widget_set_margin_start(label_bot, 0);
    gtk_box_pack_start(GTK_BOX(box), label_bot, FALSE, FALSE, 0);

    GtkWidget *bot_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_set_homogeneous(GTK_BOX(bot_box), TRUE);

    GtkWidget *start_btn = gtk_button_new_with_label("Start");
    GtkWidget *stop_btn = gtk_button_new_with_label("Stop");
    
    g_signal_connect(start_btn, "clicked", G_CALLBACK(on_start_clicked), GINT_TO_POINTER(id));
    g_signal_connect(stop_btn, "clicked", G_CALLBACK(on_stop_clicked), GINT_TO_POINTER(id));
    
    gtk_box_pack_start(GTK_BOX(bot_box), start_btn, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(bot_box), stop_btn, TRUE, TRUE, 0);
    
    gtk_box_pack_start(GTK_BOX(box), bot_box, FALSE, FALSE, 5);
  }

  {
    GtkWidget *label_char = gtk_label_new("Character:");
    gtk_label_set_xalign(GTK_LABEL(label_char), 0.0);
    gtk_widget_set_margin_start(label_char, 3);
    gtk_box_pack_start(GTK_BOX(box), label_char, FALSE, FALSE, 0);
    GtkWidget *dropdown_char = gtk_combo_box_text_new();
    for (const auto& file : characters) {
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropdown_char), file.c_str());
    }
    gtk_box_pack_start(GTK_BOX(box), dropdown_char, FALSE, FALSE, 0);
    g_signal_connect(dropdown_char, "changed", G_CALLBACK(on_character_changed), GINT_TO_POINTER(id));
  }

  {
    GtkWidget *label_waypoint = gtk_label_new("Waypoint:");
    gtk_label_set_xalign(GTK_LABEL(label_waypoint), 0.0);
    gtk_widget_set_margin_start(label_waypoint, 3);
    gtk_box_pack_start(GTK_BOX(box), label_waypoint, FALSE, FALSE, 0);
    GtkWidget *dropdown_waypoint = gtk_combo_box_text_new();
    for (const auto& file : waypoints) {
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropdown_waypoint), file.c_str());
    }
    gtk_box_pack_start(GTK_BOX(box), dropdown_waypoint, FALSE, FALSE, 0);
    g_signal_connect(dropdown_waypoint, "changed", G_CALLBACK(on_waypoint_changed), GINT_TO_POINTER(id));
  }

  gtk_container_set_border_width(GTK_CONTAINER(box), 10);

  return box;
}

void interface( int argc, char** argv ) {
  gtk_init(&argc, &argv);

  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "AFK");
  gtk_window_set_default_size(GTK_WINDOW(window), 350, 350);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  g_signal_connect(window, "destroy", G_CALLBACK(on_window_closed), NULL);

  GtkWidget *notebook = gtk_notebook_new();
  gtk_container_add(GTK_CONTAINER(window), notebook);

  for (int i = 1; i <= 6; i++) {
    GtkWidget *page = create_page(i);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page, gtk_label_new(to_string(i).c_str()));
  }

  gtk_widget_show_all(window);
  gtk_main();

}