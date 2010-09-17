<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:uml="http://schema.omg.org/spec/UML/2.1"
                xmlns:xmi="http://schema.omg.org/spec/XMI/2.1"
                xmlns:msxsl="urn:schemas-microsoft-com:xslt"
                xmlns:exslt="http://exslt.org/common">
  <xsl:output method="text"/>

  <xsl:template name="main">
    <xsl:param name="stateMachineName"/>
    <xsl:param name="enumPrefix"/>
    <xsl:param name="functionName"/>

    <xsl:variable name="enumName">
      <xsl:value-of select="$enumPrefix"/>
      <xsl:text>State</xsl:text>
    </xsl:variable>
    <xsl:variable name="root" select="."/>

    <xsl:text>#include &lt;cstdio&gt;

enum </xsl:text><xsl:value-of select="$enumName"/><xsl:text>
{
</xsl:text>
    <xsl:for-each select="xmi:XMI/uml:Model/packagedElement/packagedElement[@xmi:type='uml:StateMachine'][@name=$stateMachineName]/region/subvertex[@xmi:type='uml:State']">
      <xsl:text>	</xsl:text><xsl:value-of select="$enumPrefix"/>State<xsl:value-of select="@name"/><xsl:if test="position() != last()">,</xsl:if><xsl:text>
</xsl:text>
    </xsl:for-each>}<xsl:text>;
</xsl:text>
    <xsl:text>
bool </xsl:text><xsl:value-of select="$functionName"/><xsl:text>(FILE* in_file</xsl:text>
    <xsl:text>)
{
	// Declarations of variables
	</xsl:text><xsl:value-of select="$enumName"/><xsl:text> lCurrentState;
	fpos_t lPosition;

	// Initialization
	if (fgetpos(in_file, &amp;lPosition) != 0)
		return false;

	fseek(in_file, 0, SEEK_SET);
	
	while (1)
	{
		switch (lCurrentState)
		{
			// Declarations of variables
			unsigned char lToken;
			bool lEndOfStream = false;

			// Parser code
			if (fread(&amp;lToken, 1, 1, in_file) != 1)
				lEndOfStream = true;

</xsl:text>
    <xsl:for-each select="xmi:XMI/uml:Model/packagedElement/packagedElement[@xmi:type='uml:StateMachine'][@name=$stateMachineName]/region/subvertex[@xmi:type='uml:State']">
      <xsl:text>			case </xsl:text>
      <xsl:value-of select="$enumPrefix"/>State<xsl:value-of select="@name"/><xsl:text>:
</xsl:text>

      <xsl:variable name="transitions">
        <xsl:for-each select="region[@xmi:type='uml:Region']/outgoing/@xmi:idref">
          <xsl:variable name="current_idref" select="."/>
          <xsl:copy-of select="$root/xmi:XMI/uml:Model/packagedElement/packagedElement[@xmi:type='uml:StateMachine'][@name=$stateMachineName]/transition[@xmi:type='uml:Transition'][@xmi:id=$current_idref]"/>
        </xsl:for-each>
      </xsl:variable>

      <xsl:variable name="end_of_stream_transition" select="msxsl:node-set($transitions)/transition[trigger/@name = 'end of stream']"/>
      <xsl:variable name="else_transition" select="msxsl:node-set($transitions)/transition[trigger/@name = 'else']"/>

      <xsl:if test="count($end_of_stream_transition) = 1 or count($else_transition) = 1">
        <xsl:text>				if (lEndOfStream)
				{
</xsl:text>
        <xsl:variable name="real_end_of_stream_transition">
          <xsl:choose>
            <xsl:when test="count($end_of_stream_transition) = 1">
              <xsl:copy-of select="$end_of_stream_transition"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:copy-of select="$else_transition"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:value-of select="msxsl:node-set($real_end_of_stream_transition)/transition/effect/body"/>
        <xsl:text>
					break;
				}
</xsl:text>
      </xsl:if>

      <!-- Change exslt to msxsl (or the other way round) if necessary (for example
           for debugging in Visual Studio) -->
      <xsl:for-each select="msxsl:node-set($transitions)/transition[trigger/@name != 'end of stream'][trigger/@name != 'else']">
        <xsl:text>				</xsl:text>
        <xsl:if test="position() != 1">
          <xsl:text>else </xsl:text>
        </xsl:if>
        <xsl:text>if (</xsl:text>
        <xsl:value-of select="trigger/@name"/>
        <xsl:text>)
				{
</xsl:text>
        <xsl:value-of select="effect/body"/>
        <xsl:text>
				}
</xsl:text>
      </xsl:for-each>

      <xsl:for-each select="msxsl:node-set($transitions)/transition[trigger/@name = 'else']">
        <xsl:text>				else
				{
</xsl:text>
        <xsl:value-of select="effect/body"/>
        <xsl:text>
				}
</xsl:text>
      </xsl:for-each>
      
      <xsl:text>				break;
</xsl:text>
    </xsl:for-each>
    <xsl:text>		}
	}
}
</xsl:text>
  </xsl:template>
</xsl:stylesheet>
