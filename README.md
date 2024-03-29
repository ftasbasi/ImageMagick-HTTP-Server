![C++](https://img.shields.io/badge/-C++-000?&logo=c%2b%2b&logoColor=00599C)
<img src="https://imagemagick.org/Magick++/Magick++.png" width=100>


A simple HTTP webserver that performs on-the-fly image resizing from the provided source image url (using the ImageMagick library).

The format of the URL that is served must be as follows

Operation : “resize”

Parameters : <width>x<height>

Example
The following request should return the static.birgun.net/resim/haber-detay-resim/2020/01/30/abb-ve-odtu-den-eymir-isbirligi-681848-5.jpg resized to 640x360.

http://localhost:8080/v1/resize:640x360/url:static.birgun.net/resim/haber-detay-resim/2020/01/30/abb-ve-odtu-den-eymir-isbirligi-681848-5.jpg

Operations are ImageMagick: resize, rotate, crop, grayscale.


# SAMPLE SCREENSHOTS

Resize

![alt text](https://raw.githubusercontent.com/ftasbasi/ImageMagick-HTTP-Server/main/images/resize%3A180x80.png)

Grayscale

![alt text](https://raw.githubusercontent.com/ftasbasi/ImageMagick-HTTP-Server/main/images/grayscale.png)

Crop

![alt text](https://raw.githubusercontent.com/ftasbasi/ImageMagick-HTTP-Server/main/images/crop%3A100x80.png)

Rotate

![alt text](https://raw.githubusercontent.com/ftasbasi/ImageMagick-HTTP-Server/main/images/rotate.png)
