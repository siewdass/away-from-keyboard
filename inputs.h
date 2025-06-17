#ifndef INPUTS_H
#define INPUTS_H

#include <string>
#include <tuple>
#include <memory>
#include <stdexcept>
#include <mutex>

extern "C" {
   #include <xdo.h>
   typedef struct xdo xdo_t;
}

class Inputs {
private:
   struct XdoDeleter {
      void operator()(xdo_t* xdo) const {
         if (xdo) xdo_free(xdo);
      }
   };
   std::unique_ptr<xdo_t, XdoDeleter> xdo_;
   std::mutex xdo_mutex_;

public:
   explicit Inputs(const std::string& display = "") {
      std::lock_guard<std::mutex> lock(xdo_mutex_);
      xdo_.reset(xdo_new(display.empty() ? nullptr : display.c_str()));
      if (!xdo_) {
         throw std::runtime_error("Failed to connect to X display: " + display);
      }
   }

   ~Inputs() = default;
   Inputs(const Inputs&) = delete;
   Inputs& operator=(const Inputs&) = delete;

   void mouse(int x, int y, int button) {
      std::lock_guard<std::mutex> lock(xdo_mutex_);
      xdo_move_mouse(xdo_.get(), x, y, CURRENTWINDOW);
      xdo_mouse_down(xdo_.get(), CURRENTWINDOW, button);
      xdo_mouse_up(xdo_.get(), CURRENTWINDOW, button);
   }

   void mouseMove(int x, int y) {
      std::lock_guard<std::mutex> lock(xdo_mutex_);
      xdo_move_mouse(xdo_.get(), x, y, CURRENTWINDOW);
   }

   void mouseDrag(int x, int y) {
      std::lock_guard<std::mutex> lock(xdo_mutex_);
      xdo_move_mouse(xdo_.get(), x, y, CURRENTWINDOW);
      xdo_mouse_down(xdo_.get(), CURRENTWINDOW, 1);
      xdo_move_mouse(xdo_.get(), 497, 260, CURRENTWINDOW);
      xdo_mouse_up(xdo_.get(), CURRENTWINDOW, 1);
   }

   std::tuple<int, int> mouseLocation() {
      std::lock_guard<std::mutex> lock(xdo_mutex_);
      int X, Y, SCREEN;
      xdo_get_mouse_location(xdo_.get(), &X, &Y, &SCREEN);
      return std::make_tuple(X, Y);
   }

   void keyboard(std::string key) {
      std::lock_guard<std::mutex> lock(xdo_mutex_);
      xdo_send_keysequence_window_down(xdo_.get(), CURRENTWINDOW, key.c_str(), 0);
      xdo_send_keysequence_window_up(xdo_.get(), CURRENTWINDOW, key.c_str(), 0);
   }

   void keywith(const char* key) {
      std::lock_guard<std::mutex> lock(xdo_mutex_);
      xdo_send_keysequence_window(xdo_.get(), CURRENTWINDOW, key, 12000);
   }

   void shiftDown() {
      std::lock_guard<std::mutex> lock(xdo_mutex_);
      xdo_send_keysequence_window_down(xdo_.get(), CURRENTWINDOW, "shift", 0);
   }

   void shiftUp() {
      std::lock_guard<std::mutex> lock(xdo_mutex_);
      xdo_send_keysequence_window_up(xdo_.get(), CURRENTWINDOW, "shift", 0);
   }

   void shiftClick(int x, int y, int button) {
      std::lock_guard<std::mutex> lock(xdo_mutex_);
      
      xdo_send_keysequence_window_down(xdo_.get(), CURRENTWINDOW, "shift", 0);
      
      xdo_move_mouse(xdo_.get(), x, y, CURRENTWINDOW);
      xdo_mouse_down(xdo_.get(), CURRENTWINDOW, button);
      xdo_mouse_up(xdo_.get(), CURRENTWINDOW, button);
      
      xdo_send_keysequence_window_up(xdo_.get(), CURRENTWINDOW, "shift", 0);
   }
};

#endif