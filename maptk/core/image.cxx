/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "image.h"


namespace maptk
{

/// Default Constructor
image_memory
::image_memory()
: data_(0),
  size_(0)
{
}


/// Constructor - allocated n bytes
image_memory
::image_memory(std::size_t n)
: data_(new char[n]),
  size_(n)
{
}


/// Copy Constructor
image_memory
::image_memory(const image_memory& other)
: data_(new char[other.size()]),
  size_(other.size())
{
  std::memcpy(data_, other.data_, size_);
}


/// Destructor
image_memory
::~image_memory()
{
  delete [] reinterpret_cast<char*>(data_);
}


/// Assignment operator
image_memory&
image_memory
::operator=(const image_memory& other)
{
  if( this == &other )
  {
    return *this;
  }
  set_size(other.size());
  std::memcpy(data_, other.data_, size_);
  return *this;
}


/// Return a pointer to the allocated memory
void*
image_memory
::data()
{
  return data_;
}


/// Reallocate memory for n bytes
/// Do nothing if size has not changed
void
image_memory
::set_size(std::size_t n)
{
  if( size_ == n )
  {
    return;
  }
  delete [] reinterpret_cast<char*>(data_);
  data_ = 0;
  if (n > 0)
  {
    data_ = new char[n];
  }
  size_ = n;
}

//======================================================================

simple_image
::~simple_image()
{
}


/// Constructor that allocates image memory
simple_image
::simple_image(size_t width, size_t height, size_t depth, bool interleave)
: data_(new image_memory(width*height*depth)),
  first_pixel_(reinterpret_cast<byte*>(data_->data())),
  width_(width),
  height_(height),
  depth_(depth),
  w_step_(1),
  h_step_(width),
  d_step_(width*height)
{
  if( interleave )
  {
    d_step_ = 1;
    w_step_ = depth;
    h_step_ = depth*width;
  }
}


/// Constructor that points at existing memory
simple_image
::simple_image(const byte* first_pixel, size_t width, size_t height, size_t depth,
               ptrdiff_t w_step, ptrdiff_t h_step, ptrdiff_t d_step)
: data_(),
  first_pixel_(const_cast<byte*>(first_pixel)),
  width_(width),
  height_(height),
  depth_(depth),
  w_step_(w_step),
  h_step_(h_step),
  d_step_(d_step)
{
}


/// Const access to the image data
const image::byte*
simple_image
::data() const
{
  if( !data_ )
  {
    return NULL;
  }
  return reinterpret_cast<const byte*>(data_->data());
}


/// Access to the image data
image::byte*
simple_image
::data()
{
  if( !data_ )
  {
    return NULL;
  }
  return reinterpret_cast<byte*>(data_->data());
}


/// The size of the image data in bytes
/// This size includes all allocated image memory,
/// which could be larger than width*height*depth.
size_t
simple_image
::size() const
{
  if( !data_ )
  {
    return 0;
  }
  return data_->size();
}


} // end namespace maptk
