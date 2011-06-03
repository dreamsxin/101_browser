<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:h="http://www.w3.org/1999/xhtml">
  <xsl:import href="replace.xslt"/>
  <xsl:output method="text"/>

  <xsl:template name="main">
    <xsl:param name="moduleName"/>
    <xsl:text>module </xsl:text><xsl:value-of select="$moduleName"/><xsl:text>
{
</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>
};
</xsl:text>
  </xsl:template>

  <!-- For geolocation, web-apps 1.0 -->
  <xsl:template match="h:pre[@class='idl']">
    <xsl:variable name="text">
      <xsl:value-of select="."/>
    </xsl:variable>
    <xsl:text>  </xsl:text>
    <xsl:call-template name="string-replace-all">
      <xsl:with-param name="text" select="$text" />
      <xsl:with-param name="replace" select="'&#xA;'" />
      <xsl:with-param name="by" select="'&#xA;  '" />
    </xsl:call-template>
    <xsl:text>

</xsl:text>
  </xsl:template>
  
  <!-- For IndexedDB (not working properly, since there are elements to add - TODO) -->
  <xsl:template match="h:dl[@class='idl']/h:dt">
    <xsl:variable name="text">
      <xsl:value-of select="."/>
    </xsl:variable>
    <xsl:text>  </xsl:text>
    <xsl:call-template name="string-replace-all">
      <xsl:with-param name="text" select="$text" />
      <xsl:with-param name="replace" select="'&#xA;'" />
      <xsl:with-param name="by" select="'&#xA;  '" />
    </xsl:call-template>
    <xsl:text>

</xsl:text>
  </xsl:template>

  <xsl:template match="*">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="text()"/>
</xsl:stylesheet>
