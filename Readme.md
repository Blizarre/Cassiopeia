# Cassiopeia

Merge multiple RAW images with motion correction. 

The goal is to be able to take pictures of the night sky without a equatorial mount. The stars moves in the sky which generate [star trails](https://en.wikipedia.org/wiki/Star_trail) in pictures and blur any larger structure.

By:
- taking multiple pictures with a short exposition time
- realigning (using a non-linear transformation) the stars within all pictures 
- merging all the RAW data into the final picture
I hope to be able to see the milky way.

Working directly from the RAW file allows a more direct access to the sensor data, before post-processing, jpeg compression, gamma correction, which would all reduce the amount of informations available in the dark regions of the sky.

- Raw file Handling: LibRaw (`apt install libraw-dev` tested with `0.18.8-1ubuntu0.1`)
- Images alignment: OpenCV (`apt install libopencv-dev` tested with `3.2.0+dfsg-4ubuntu0.1`)