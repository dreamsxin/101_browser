<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:uml="http://schema.omg.org/spec/UML/2.1"
                xmlns:xmi="http://schema.omg.org/spec/XMI/2.1"
                xmlns:exslt="http://exslt.org/common"
                xmlns:str="http://exslt.org/strings">
  <xsl:import href="replace.xslt"/>
  <xsl:output method="text"/>

  <xsl:template name="create_transition_code">
    <xsl:param name="enumPrefix"/>
    <xsl:param name="root"/>
    <xsl:param name="transition"/>

    <xsl:text>					</xsl:text>

    <xsl:choose>
      <xsl:when test="not($transition/@kind['internal'] = 'internal')">
        <xsl:text>lCurrentState = </xsl:text>
        <xsl:value-of select="$enumPrefix"/>
        <xsl:text>State</xsl:text>

        <xsl:variable name="targetId" select="$transition/@target"/>
        
        <xsl:value-of select="$root/xmi:XMI/uml:Model/packagedElement/packagedElement[@xmi:type='uml:StateMachine']/region/subvertex[@xmi:type='uml:State'][@xmi:id=$targetId]/@name"/>
        <xsl:text>;
</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>// internal transition - no state change
</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="create_body">
    <xsl:param name="body"/>

    <xsl:if test="$body">
      <xsl:variable name="splitBody">
        <xsl:call-template name="string-replace-all">
          <xsl:with-param name="text" select="$body"/>
          <xsl:with-param name="replace" select="'&#10;'"/>
          <xsl:with-param name="by" select="'&#10;&#9;&#9;&#9;&#9;&#9;'"/>
        </xsl:call-template>
      </xsl:variable>

      <xsl:text>					</xsl:text>
      <xsl:value-of select="$splitBody"/>
      <xsl:text>
</xsl:text>
    </xsl:if>
  </xsl:template>

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

      <xsl:variable name="end_of_stream_transition" select="exslt:node-set($transitions)/transition[trigger/@name = 'end of stream']"/>
      <xsl:variable name="else_transition" select="exslt:node-set($transitions)/transition[trigger/@name = 'else']"/>

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

        <xsl:call-template name="create_body">
          <xsl:with-param name="body" select="exslt:node-set($real_end_of_stream_transition)/transition/effect/body"/>
        </xsl:call-template>
        
        <xsl:call-template name="create_transition_code">
          <xsl:with-param name="enumPrefix" select="$enumPrefix"/>
          <xsl:with-param name="root" select="$root"/>
          <xsl:with-param name="transition" select="exslt:node-set($real_end_of_stream_transition)/transition"/>
        </xsl:call-template>
        <xsl:text>					break;
				}
</xsl:text>
      </xsl:if>

      <xsl:for-each select="exslt:node-set($transitions)/transition[trigger/@name != 'end of stream'][trigger/@name != 'else']">
        <xsl:text>				</xsl:text>
        <xsl:if test="position() != 1">
          <xsl:text>else </xsl:text>
        </xsl:if>
        <xsl:text>if (</xsl:text>
        <xsl:value-of select="trigger/@name"/>
        <xsl:text>)
				{
</xsl:text>
        <xsl:call-template name="create_body">
          <xsl:with-param name="body" select="effect/body"/>
        </xsl:call-template>

        <xsl:call-template name="create_transition_code">
          <xsl:with-param name="enumPrefix" select="$enumPrefix"/>
          <xsl:with-param name="root" select="$root"/>
          <xsl:with-param name="transition" select="."/>
        </xsl:call-template>
        
        <xsl:text>				}
</xsl:text>
      </xsl:for-each>

      <xsl:for-each select="exslt:node-set($transitions)/transition[trigger/@name = 'else']">
        <xsl:text>				else
				{
</xsl:text>
        <xsl:call-template name="create_body">
          <xsl:with-param name="body" select="effect/body"/>
        </xsl:call-template>
        
        <xsl:call-template name="create_transition_code">
          <xsl:with-param name="enumPrefix" select="$enumPrefix"/>
          <xsl:with-param name="root" select="$root"/>
          <xsl:with-param name="transition" select="."/>
        </xsl:call-template>
        
        <xsl:text>				}
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
