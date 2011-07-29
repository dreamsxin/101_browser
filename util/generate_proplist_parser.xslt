<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:uml="http://schema.omg.org/spec/UML/2.1" xmlns:xmi="http://schema.omg.org/spec/XMI/2.1">
  <xsl:import href="generate_source_from_xmi.xslt"/>
  <xsl:output method="text"/>

  <xsl:template match="/">
    <xsl:call-template name="main">
      <xsl:with-param name="stateMachineName">PropListParserStateMachine</xsl:with-param>
      <xsl:with-param name="enumPrefix">PropListParser</xsl:with-param>
      <xsl:with-param name="functionName">readPropList</xsl:with-param>
      <xsl:with-param name="include_text">
        <xsl:text>#include "Unicode/PropList.h"
#include "Util/Interval.hpp"
#include "Util/Unicode.h"
#include &lt;cstdlib&gt; // for malloc
#include &lt;list&gt;
#include &lt;stdexcept&gt;
using namespace std;</xsl:text>
      </xsl:with-param>
      <xsl:with-param name="parameters">
        <parameter>
          <xsl:text>const char* in_property</xsl:text>
        </parameter>
        <parameter>
          <xsl:text>void* out_ppIntervals</xsl:text>
        </parameter>
        <parameter>
          <xsl:text>size_t* out_intervalsCount</xsl:text>
        </parameter>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>
</xsl:stylesheet>