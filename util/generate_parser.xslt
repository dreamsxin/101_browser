<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:uml="http://schema.omg.org/spec/UML/2.1" xmlns:xmi="http://schema.omg.org/spec/XMI/2.1">
  <xsl:import href="generate_source_from_xmi.xslt"/>
  <xsl:output method="text"/>

  <xsl:template match="/">
    <xsl:call-template name="main">
      <xsl:with-param name="stateMachineName">PropListParserStateMachine</xsl:with-param>
      <xsl:with-param name="enumPrefix">PropListParser</xsl:with-param>
    </xsl:call-template>
  </xsl:template>
</xsl:stylesheet>