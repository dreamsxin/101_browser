<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:template name="string-split">
    <xsl:param name="text" />
    <xsl:param name="separator" />
    <xsl:choose>
      <xsl:when test="contains($text, $separator)">
        <token>
          <xsl:value-of select="substring-before($text, $separator)" />
        </token>
        <xsl:call-template name="string-split">
          <xsl:with-param name="text" 
                          select="substring-after($text, $separator)" />
          <xsl:with-param name="separator" select="$separator" />
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <token>
          <xsl:value-of select="$text" />
        </token>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>
