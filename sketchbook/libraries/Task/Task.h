#ifndef TASK_H
#define TASK_H

class Task {
  virtual bool run(unsigned int ms_delta) = 0;
};

#endif
