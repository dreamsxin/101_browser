<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:h="http://www.w3.org/1999/xhtml">
  <xsl:output method="text"/>

  <xsl:include href="create_dynamic_idl.xslt"/>

  <xsl:template match="/">
    <xsl:call-template name="main">
      <xsl:with-param name="moduleName">websockets</xsl:with-param>
    </xsl:call-template>
  </xsl:template>
</xsl:stylesheet>