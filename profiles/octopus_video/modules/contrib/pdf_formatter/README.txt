PDF Formatter

PDF Formatter provides two formatters for dealing with PDF files. These
formatters use the pdftotext and pdftohtml utilities. Under Ubuntu, they
come with the poppler-utils package. Though PDF Formatter has been
developped under Linux, it should work under Windows as long as these
utilities are installed on the system.

Under Drupal 7, a formatter can be assigned to a combination of field and
display (Structure->Content Types->Manage display).

The available formatters are :
- Convert PDF to text
- Convert PDF to HTML

The generated output is crude because PDF Formatter aimed at making PDF file
attachment indexable by the search module. Therefore, PDF Formatter should be
used on the 'Search index' or 'Search result' displays. Using it on other
displays will work but will not provide a satisfactory experience.

You may have to enable Custom display settings for 'Search index' and
'Search result'.

That's all !

There is no table, no variable, no menu, no settings page.

