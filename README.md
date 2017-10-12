# image-search
### Cpp + OpenCV 3.0 implementation of a local, directory-based image search engine

This application was created as a part of my undergraduate course project:
> Template image searching based on perceptional color proximity

Each class submerges into different steps of searching:
1. Detecting user-specified template (object of interest)
2. Calculating average color of a region based on a mask, retained from object detection
3. Converting pixel color spaces (RGB &#8596; CIEXYZ &#8596; CIEL\*a\*b)
4. Calculating &#916;E - color difference
5. Applying general search routines:
   1. Iterating over files in specified directory
   2. Retaining image ROI
   3. Calculating &#916;E
   4. Indexing
   5. Outputing the result
