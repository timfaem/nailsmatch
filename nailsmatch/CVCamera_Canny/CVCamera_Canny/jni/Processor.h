/*
 * Processor.h
 */

#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>

#include "image_pool.h"

#define DETECT_CANNY 0
#define DETECT_HOUGH_LINES 1
#define DETECT_HOUGH_CIRCLES 2

class Processor
{
public:

  Processor();
  virtual ~Processor();

  void processImage(int idx, image_pool* pool, int feature_type);
  
private:
  
};

#endif /* PROCESSOR_H_ */
