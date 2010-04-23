<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:h="http://www.w3.org/1999/xhtml">
  <xsl:output method="text"/>
  
  <xsl:template match="//h:p">
    <xsl:if test="contains(@class, 'element-syntax')">
	  <xsl:value-of select="."/>
	  <xsl:text>

</xsl:text>
	</xsl:if>
  </xsl:template>
  
  <xsl:template match="text()"/>
</xsl:stylesheet>