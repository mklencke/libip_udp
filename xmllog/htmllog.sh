#!/bin/sh
`dirname $0`/xmllog | xsltproc `dirname $0`/html/htmllog.xsl -

