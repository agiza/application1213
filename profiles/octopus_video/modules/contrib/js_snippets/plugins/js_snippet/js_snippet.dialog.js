
(function($) {

var JsSnippetDialog = {
    init : function() {
        // We could do something with selected text
        var node = tinyMCEPopup.editor.selection.getNode();

//        if (typeof node == 'object' && $(node).is('div.js_snippet')) {
//          snippetId = node.getAttribute('data-snippet-id');
//        }

    },
    insert : function(id, title, snippetText) {

        var node = tinyMCEPopup.editor.selection.getNode();
        node.remove();
        tinyMCEPopup.editor.execCommand('mceReplaceContent', false, this.formatSnippet(id, title, snippetText));

        var ed = tinyMCE.activeEditor;

        //add an empty span with a unique id
        var endId = tinymce.DOM.uniqueId();
        ed.dom.add(ed.getBody(), 'span', {'id': endId}, '');

        //select that span
        var newNode = ed.dom.select('span#' + endId);
        ed.selection.select(newNode[0]);

        tinyMCEPopup.close();

    },
    formatSnippet : function (id, title, snippetText) {
        return '<div class="js_snippet" data-snippet-id="' + id + '" data-snippet-text="' + encodeURI(title) + '">' + snippetText + '</div>';
    }
};


tinyMCEPopup.onInit.add(JsSnippetDialog.init, JsSnippetDialog);


Drupal.behaviors.attachJsSnippetsDialog = {
    attach: function (context, settings) {

        // Turn on tabs
        $('#snippet-tabs', context).tabs();

        // Attach behaviors to our links
        $('#snippet-library', context).find('a').once('snippet-list', function () {
            $(this).each(function (index, item) {
                $(item).bind('click', function (e) {
                    e.preventDefault();
                    var url = $(this).attr('href').replace("/js_snippet/rendered/", "");
                    var title = $(this).attr('title');
                    JsSnippetDialog.insert(url, title, title);
                });
            });
        });
    }
};

Drupal.ajax.prototype.commands.insertSnippet = function(ajax, response, status) {
    JsSnippetDialog.insert(response.id, response.title, response.title);
};
})(jQuery);

