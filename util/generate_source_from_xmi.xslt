<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:uml="http://schema.omg.org/spec/UML/2.1" xmlns:xmi="http://schema.omg.org/spec/XMI/2.1">
  <xsl:output method="text"/>

  <xsl:template name="main">
    <xsl:param name="stateMachineName"/>
    <xsl:param name="enumPrefix"/>
    <xsl:text>enum </xsl:text><xsl:value-of select="$enumPrefix"/><xsl:text>State
{
</xsl:text>
    <xsl:for-each select="xmi:XMI/uml:Model/packagedElement/packagedElement[@xmi:type='uml:StateMachine'][@name=$stateMachineName]/region/subvertex[@xmi:type='uml:State']">
      <xsl:text>	</xsl:text><xsl:value-of select="$enumPrefix"/>State<xsl:value-of select="@name"/><xsl:if test="position() != last()">,</xsl:if><xsl:text>
</xsl:text>
    </xsl:for-each>}<xsl:text>;</xsl:text>
    <xsl:text>

</xsl:text>
    <xsl:text>
{
</xsl:text>
    <xsl:for-each select="xmi:XMI/uml:Model/packagedElement/packagedElement[@xmi:type='uml:StateMachine'][@name=$stateMachineName]/region/subvertex">


    </xsl:for-each>
    <xsl:text>}
</xsl:text>
  </xsl:template>
</xsl:stylesheet>
