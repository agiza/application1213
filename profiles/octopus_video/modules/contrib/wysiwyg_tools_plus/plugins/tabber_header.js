(function ($) {
	Drupal.wysiwyg.plugins.tabber_header = {
	  invoke: function(data,settings,instanceId) {
			Drupal.wysiwyg.instances[instanceId].insert('<h4 class="ready-tabber-header">' + data.content + '</h4>');
		},
	}
}(jQuery));