;(function($) {

Drupal.behaviors.attachJsSnippets = {
    renderClosureOpening : function() {
        return '' +
            'jQuery(document).ready(function($) {' +
            "\n" +
            '';
    },
    renderClosureClosing : function() {
        return "\n" +
            '});' +
            "\n";
    },
    attach: function (context, settings) {

        // Popup form Behaviors (since #states is such a pain in the arse)
        // Hide Library Info if not ticked or forced
        $('form :input[name="library"]').bind('change', Drupal.behaviors.attachJsSnippets.toggleLibraryFields);
        this.toggleLibraryFields();

        // Apply behaviors to Data Field element
        this.toggleDataFields();

        // Add a little jQuery wrapper helper button
        $('fieldset.js-snippets-snippet-wrapper', context).each(function (index, item) {
            var label = $(item).find('label[for="edit-snippets-' + index + '-data-inline"]');
            $button = $('<a href="#add-jq-' + index + '">Add jQuery wrapper</a>');
            $(label).after($button);
            $button.bind('click', function(e) {
                e.preventDefault();
                var textarea = $(item).find('textarea#edit-snippets-' + index + '-data-inline');
                textarea.val(
                        Drupal.behaviors.attachJsSnippets.renderClosureOpening() +
                        textarea.val() +
                        Drupal.behaviors.attachJsSnippets.renderClosureClosing()
                );
            });
        });
    },

    toggleLibraryFields: function() {

        var element = $('form :input[name="library"]');
        var show = false;

        switch (element.attr("type")) {
            case 'checkbox':
                if (element.is(':checked')) {
                    show = true;
                }
                break;
            case 'hidden':
                if (element.attr('value') == 1) {
                    show = true;
                }
                break;
        }

        if (show) {
            $('fieldset#edit-library-info').show();
        }
        else {
            $('fieldset#edit-library-info').hide();
        }
    },
    toggleDataFields: function() {

        $('.js-snippets-snippet-wrapper').each(function (index, elem) {

            // Get the index of the current snippet wrapper
            // (probably the same as index here, but can't be sure)
            var wrapperClasses = $(this).attr('class').split(/\s+/);
            $.each(wrapperClasses, function (index, elem) {
                if (elem.indexOf("snippet-id") === 0) {
                    index = elem.replace("snippet-id-", "");
                }
            });

            var typeElement = $(':[name="snippets[' + index + '][type]"]');
            var type = typeElement.attr('value');

            if (typeElement.is('select')) {
                typeElement.bind('change', function (e) {
                    var t = $(this).attr('value');
                    var i = $(this).attr('name').replace("snippets[", "").replace("][type]", "");
                    Drupal.behaviors.attachJsSnippets.toggleDataField(i, t);
                });
            }

            Drupal.behaviors.attachJsSnippets.toggleDataField(index, type);

        });

    },
    toggleDataField: function (index, type) {

        var types = ['injected', 'file', 'external', 'inline'];
        $.each(types, function (i, elem) {
            var inputName = ':input[name="snippets[' + index + '][data_' + elem + ']"]';
            var parent = $(inputName).parents('.form-item');
            if (type == elem) {
                $(parent).show();
            }
            else {
                $(parent).hide();
            }
        });
    }
};

})(jQuery);

