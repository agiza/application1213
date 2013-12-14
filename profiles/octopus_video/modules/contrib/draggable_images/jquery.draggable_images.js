/**
 * @file
 * Draggable Images JQuery plugin
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 * Settings:
 * - 
 * - 
 *
 */

(function($){

  // Process responsive_background
  $.fn.draggable_images = function(options){
    var settings = $.extend($.fn.draggable_images.defaults, options);
    var images = this;

    var imageContainerWidth = $(window).width(),
        imageContainerHeight = $(window).height();
    
    var vertical = settings['vertical'], horizontal = settings['horizontal'];
 
    $('#draggable-images-wrapper').css({ position: settings['position'], width: settings['width'] + 'px', height: settings['height'] + 'px' });  
    
    // vertical
    if(vertical = 'top') {
      $('#draggable-images-wrapper').css({ top: 0, bottom: 'auto' }); 
    }
    else {
      $('#draggable-images-wrapper').css({ top: 'auto', bottom: 0 });     
    }   
    
    // horizontal
    if(horizontal = 'right') {
      $('#draggable-images-wrapper').css({ right: 0, left: 'auto', float: 'right' }); 
    }
    else {
      $('#draggable-images-wrapper').css({ right: 'auto', left: 0, float: 'left' });     
    }   
                
    // Load on refresh
    var idleTime = 0;
    // intialize
    makeDraggable();

    // Load on resize
    $(window).resize(function(e){
     
    });
              
    return this;
  };
  
  $.fn.draggable_images.defaults = {
    position: 'absolute',
    vertical: 'top',
    horizontal: 'right',
    width: 960,
    height: 400,
    image_style: 'medium' 
  };

// idle
function timerIncrement() {
  idleTime++;

  if (idleTime > 1) { // 6 sec
    //alert(idleTime);
    makeDraggable();  
  }
}


function makeDraggable() {
	// When everything has loaded, place all polaroids on a random position	
	$("#draggable-images-wrapper .draggable-image-container").each(function (i) {
		var tempVal = Math.round(Math.random());
		if(tempVal == 1) {
			var rotDegrees = randomXToY(340, 350); // rotate left
		} else {
			var rotDegrees = randomXToY(0, 30); // rotate right
		}

		var wiw = Math.round($(window).width() - 100);
		var wih = $(window).height();	
		
		// Internet Explorer doesn't have the "window.innerWidth" and "window.innerHeight" properties
		if(window.innerWidth == undefined) { 
			var wiw = 1000;
			var wih = 700;
		} else {
			var wiw = window.innerWidth;
			var wih = window.innerHeight;	
		}
		
	  
	  $("#block-system-main .field-name-field-gallery").width(wiw + 100).height(800);
	  var right = Math.round(Math.random()*(wiw * .35));
	  var top = Math.round(Math.random()*30);
		var cssObj = { 
		  'right' : right,
			'top' : top,
		  //'left' : Math.random()*(wiw-400),
			//'top' : Math.random()*(wih-400),
			'-moz-transform' : 'rotate('+ rotDegrees +'deg)',
			'-webkit-transform' : 'rotate('+ rotDegrees +'deg)',  // safari only
			'tranform' : 'rotate('+ rotDegrees +'deg)',
			'z-index' : Math.round(Math.random() * 5) }; // added in case CSS3 is standard
		$(this).animate({'right' : right, 'top' : top}, 500);
		$(this).css(cssObj);
	});
	
	// Set the Z-Index (used to display images on top while dragging)
	var zindexnr = 1;
	
	// boolean to check if the user is dragging
	var dragging = false;
	
	// Show the polaroid on top when clicked on
	$("#draggable-images-wrapper .draggable-image-container").mouseup(function(e){
		if(!dragging) {
			// Bring polaroid to the foreground
			zindexnr++;
			var cssObj = { 'z-index' : zindexnr,
			'transform' : 'rotate(0deg)',	 // added in case CSS3 is standard
			'-moz-transform' : 'rotate(0deg)', // firefox
			'-webkit-transform' : 'rotate(0deg)' };  // safari only
			$(this).css(cssObj);
		}
	});
	
	// Make the image draggable & display a shadow when dragging
	$("#draggable-images-wrapper .draggable-image-container").each(function() {
	  $(this).addClass('ui-widget-content');
	  $(this).draggable({
	    containment: $("#draggable-images-wrapper"),
		  cursor: 'crosshair',
		  start: function(event, ui) {
			  dragging = true;
			  zindexnr++;
			  var cssObj = { 
			    //'box-shadow' : '#888 5px 10px 10px', // added in case CSS3 is standard
				  //'-webkit-box-shadow' : '#888 5px 10px 10px', // safari only
				  'margin-left' : '-10px',
				  'margin-top' : '-10px',
				  'z-index' : zindexnr };
			  $(this).css(cssObj);
		  },
		  delay: 250,
		  stop: function(event, ui) {
			  var tempVal = Math.round(Math.random());
			  if(tempVal == 1) {
				  var rotDegrees = randomXToY(330, 360); // rotate left
			  } else {
				  var rotDegrees = randomXToY(0, 30); // rotate right
			  }
			  var cssObj = { 
			    //'box-shadow' : '', // added in case CSS3 is standard
				  //'-webkit-box-shadow' : '', // safari only
				  'transform' : 'rotate('+ rotDegrees +'deg)', // added in case CSS3 is standard
			    '-moz-transform' : 'rotate('+ rotDegrees +'deg)',
				  '-webkit-transform' : 'rotate('+ rotDegrees +'deg)', // safari only
				  'margin-left' : '0px',
				  'margin-top' : '0px' };
			  $(this).css(cssObj);
			  dragging = false;
		  }
	  });
	});
	
	// Function to get random number upto m
	// http://roshanbh.com.np/2008/09/get-random-number-range-two-numbers-javascript.html
	function randomXToY(minVal,maxVal,floatVal) {
		var randVal = minVal+(Math.random()*(maxVal-minVal));
		return typeof floatVal=='undefined'? Math.round(randVal): randVal.toFixed(floatVal);
	}
}

})(jQuery); 
