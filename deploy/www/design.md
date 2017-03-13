# Webserver design doc
### // TODO: Team Name

For project 9, we added two features to our webserver: markdown-to-HTML rendering
and a MySQL server interface.

---

## Feature #1: Markdown rendering

We used [cpp-markdown](https://sourceforge.net/projects/cpp-markdown/) to
convert Markdown files to HTML because it was the easiest to understand and 
was written in the same style as the rest of our project (C++ / boost).
To integrate Markdown rendering into our static file handler, all we had to do
was create a `markdown::Document` document for any files ending with the file
extension `.md`, read our file content into the doc, then get the resulting
HTML string. We additionally updated our static handler unit tests to ensure
that the handler would output the correct HTML when provided a simple markdown
file.

[Sample markdown file served by our server](http://54.190.63.110:8080/static1/markdown.md)

The markdown equivalent of that file can be found
[here](https://github.com/UCLA-CS130/TODO-Team-Name/blob/master/deploy/www/markdown.md)
(Github automatically renders `.md` files to HTML, but you can select 'raw' to see
what the raw file looks like).

This design document is an example of a Markdown file that has been converted by our
webserver to HTML!

---

## Feature #2: MySQL interface
Include: feature description, high-level details of implementation,
URL for demoing that feature, and an exact walkthrough that instructors can follow along
