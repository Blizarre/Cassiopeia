# Star cleaner

Merge multiple RAW images with motion correction. 

THe goal is to be able to take pictures of the night sky without a equatorial mount. The stars moves in the sky which generate "star trails" in pictures and blur any larger structure.

By taking multiple pictures with a short exposition time, and realigning (non-linear) the stars within all pictures before merging the RAW data into the final picture, I hope to be able to see the milky way.

Working directly from the RAW file allows us to access the whole data, before post-processing, jpeg compression, gamma correction, which will all reduce the amount of informations available in the dark regions of the sky.

- Raw file Handling: LibRaw
- Images alignment: OpenCV