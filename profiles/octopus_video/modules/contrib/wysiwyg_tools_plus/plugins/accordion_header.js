(function ($) {
	Drupal.wysiwyg.plugins.accordion_header = {
	  invoke: function(data,settings,instanceId) {
			Drupal.wysiwyg.instances[instanceId].insert('<span class="ready-accordion-header">' + data.content + '</span>');
		},
	}
}(jQuery));