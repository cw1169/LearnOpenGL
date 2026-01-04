#include "texture_handler.hpp"

#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>

unsigned int load2DTexture(const char *path) {
  // generate a texture ID and bind it to the GL_TEXTURE_2D target
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Handles texture wrapping if needed
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  // Texture Filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR); // uses linear filtering for
                                            // minification between mipmaps
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR); // use linear filtering for magnification

  // loading texture using stb_image
  int width, height, nrChannels;
  unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

  // generate texture
  if (data) {
    if (nrChannels == 3) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE,
                   data); // (texture target, mipmap layer, storage format,
                          // width, height, legacy stuff, source format, source
                          // datatype, image data)
      glGenerateMipmap(GL_TEXTURE_2D); // generate mipmaps
                                       //
    } else if (nrChannels == 4) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE,
                   data); // (texture target, mipmap layer, storage format,
                          // width, height, legacy stuff, source format, source
                          // datatype, image data)
      glGenerateMipmap(GL_TEXTURE_2D); // generate mipmaps
    }
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data); // free the image data after generating the texture
  return texture;
}
