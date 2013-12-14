
(function ($) {

/**
 * Wysiwyg plugin button implementation for js-snippet plugin.
 */
Drupal.wysiwyg.plugins.js_snippet = {
    /**
     * Return whether the passed node belongs to this plugin.
     */
    isNode: function (node) {
        $node = this.getRepresentitiveNode(node);

        if ($node.is('div.js_snippet')) {
            return true;
        }

        $.each($node.parents(), function (index, elem) {
            if ($(elem).hasClass('js_snippet')) {
                return true;
            }
        });

        return false;
    },

    /* We need this due all the special cases in the editors */
    getRepresentitiveNode: function(node) {
        if (node != null && typeof(node) != 'undefined' && '$' in node) {
            // This case is for the CKeditor, where
            // $(node) != $(node.$)
            return $(node.$);
        }
        // else
        // This would be for the TinyMCE and hopefully others
        return $(node).is('div.js_snippet') ? $(node) : $(node).parent('div.js_snippet');
    },

    /**
     * Execute the button.
     */
    invoke: function (data, settings, instanceId) {

        if(data == null && typeof(data) == 'undefined') {
            return;
        }

        if (data.format == 'html') {
            // Default
            var options = {
                id: instanceId,
                action: 'insert'
            };
            var $node = null;
            if ('node' in data) {
                 $node = this.getRepresentitiveNode(data.node);
            }

            if ($node != null && $node.is('div') && $node.hasClass('js_snippet')) {
                options.action = 'update';
                options.snippetId = $node.attr('data-snippet-id');
            }
        }
        else {
            // @todo Plain text support.
        }
        // Add or update.
        if (options.action == 'insert' || options.action == 'update') {

            var snippet = options.snippetId ? options.snippetId : 'add';
            var editor = tinyMCE.get(instanceId);
            editor.windowManager.open({
                file: settings.dialog.url + snippet,
                width: settings.dialog.width,
                height: settings.dialog.height,
                inline: 1,
                scrollbars: true
            });


//            Drupal.wysiwyg.instances[instanceId].openDialog(settings.dialog)
        }
    },

    /*
     * Thats the most critical part. Call the WYSIWYG API to insert this html into
     * the current editor, no matter what editor it might be
     */
    insertIntoEditor: function (data, editor_id) {
        // This is all the magic
        Drupal.wysiwyg.instances[editor_id].insert(data);
    },
    formatSnippet : function (id, title) {
        return '<div class="js_snippet" data-snippet-id="' + id + '" data-snippet-text="' + encodeURI(title) + '">' + title + '</div>';
    },
//    renderSnippet : function(id) {
//    },
    attach: function(content, pluginSettings, id) {
        var plugin = this;
        content = content.replace(
            /\[\{\[js_snippet:(\d+):([^\]]*?)\]\}\]/g,
            function(original, snippetId, text) {
                return plugin.formatSnippet(snippetId, decodeURI(text));
            }
        );
        return content;
    },

    detach: function (content, pluginSettings, id)  {
        content = '<div>'+content+'</div>';
        var $content = $(content);
        $content.find('div.js_snippet').map(
            function(i, div) {
                var $div = $(div);

                // Thats the inlineID we use for extracting the meta data from the database
                var inlineID = $div.attr('data-snippet-id');
                var inlineText = $div.attr('data-snippet-text');
                $(div, $content).replaceWith('[{[js_snippet:'+inlineID+':' + inlineText + ']}]');
            }
        );
        content = $content.html();
        $content.remove();
        return content;
    }

}

})(jQuery);