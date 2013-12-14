Drupal jQZoom module:
--------------------
Original Author - Matt Vance (mvance at pobox dot com)
Updated for Drupal 7 - wesnick
License - GPL


Overview:
--------
The jQZoom module is a wrapper for the jQuery plugin jQZoom.
The jQuery library is a part of Drupal since version 5.

* jQuery - http://jquery.com/
* jQZoom - http://www.mind-projects.it/projects/jqzoom/

From the jQZoom homepage:
"jQZoom allows you to realize a small magnifier window close to the image or
images on your web page easily. I decided to build this jQuery plugin to embed
detailed big images in my B2B. So now in few steps you can have your jQZoom
in your website,eCommerce or whatever you want."


Installation:
------------

1) Start by copying the module files into your 'modules' directory.  For more
information: http://drupal.org/documentation/install/modules-themes/modules-7

2) The jQZoom module utilizes the Libraries module. If not already installed, 
download, enable, and configure the latest versions of the Libraries module. 
See: http://drupal.org/project/libraries

3) Download jQZoom from http://www.mind-projects.it/projects/jqzoom/ and unzip
the archive into your site's 'libraries' directory (usually /site/all/libraries)
Once it is there, you should rename the jqzoom directory "jqzoom".  The 
resulting path should be sites/{all}/libraries/jqzoom and you should be albe to
find the javascript file in 
"sites/{all}/libraries/jqzoom/js/jquery.jqzoom-core-pack.js"

4) On the Modules > List page, enable the jQZoom module.

5) Enable an image toolkit module (either ImageAPI GD2 or ImageAPI ImageMagick).

6) Select Configuration > Media > Image Styles.  Add 2 new presets. One preset
will be for the unzoomed images, the other for the zoomed image.  You should 
care to give them names that you can recognize as zoomed or unzoomed.

7) From the field management area, you can "Manage Display" of an Image field
you wish to configure to use jQZoom. Specify the presets that you wish to use 
when displaying the image and the zoom, respectively.

Contributions:
-------------
* The original module is largely based on the Thickbox module. Many thanks to 
  all the Thickbox developers and contributors.
* Thanks to surge_martin on drupal.org for updating the module to Drupal 6 and
  for adding an adminitration screen for specifying configuration options. 

Last updated:
------------
Feb. 12, 2012 by wesnick