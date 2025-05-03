#ifndef INPUTS_H
#define INPUTS_H

#include <string>
#include <tuple>
#include <memory>

// Use the exact same declaration as xdo.h
extern "C" {
   typedef struct xdo xdo_t;
}

class Inputs {
private:
   struct XdoDeleter {
      void operator()(xdo_t* xdo) const;
   };
   std::unique_ptr<xdo_t, XdoDeleter> xdo_;

public:
   explicit Inputs(const std::string& display = "");
   void mouse(int x, int y, int button);
   void mouseMove(int x, int y);
   void mouseDrag(int x, int y);
   std::tuple<int, int> mouseLocation();
   void keyboard(std::string key);
   void keywith(const char* key);
   void shiftDown();
   void shiftUp();

   ~Inputs() = default;
   Inputs(const Inputs&) = delete;
   Inputs& operator=(const Inputs&) = delete;
};

#endif