(function ($) {
	Drupal.wysiwyg.plugins.tabber = {
	  invoke: function(data,settings,instanceId) {
 			Drupal.wysiwyg.instances[instanceId].insert('<div class="ready-tabber">&nbsp;' + data.content + '</div>');
		},
	}
}(jQuery));