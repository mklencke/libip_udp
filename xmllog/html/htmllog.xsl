<?xml version="1.0" encoding="UTF-8" ?>

<!-- XSL Stylesheet for xmllog -> htmllog

`xsltproc $thisfile $xmllogfile > $htmllogfile`

-->

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:variable name="side1" select="'192.168.0.1'"/>
	<xsl:variable name="side2" select="'192.168.0.2'"/>

	<xsl:template match="/">
		<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
			<head>
				<link rel="stylesheet" href="htmllog.css" title="Normal" />
				<title><xsl:text>HTMLLOG</xsl:text></title>
			</head>
			<body>
				<xsl:apply-templates select="log"/>
			</body>
		</html>
	</xsl:template>


	<xsl:template match="psh">
		<xsl:text>PSH </xsl:text>
	</xsl:template>

	<xsl:template match="syn">
		<xsl:text>SYN </xsl:text>
	</xsl:template>

	<xsl:template match="fin">
		<xsl:text>FIN </xsl:text>
	</xsl:template>

	<xsl:template match="ack">
		<xsl:text>ACK </xsl:text>
	</xsl:template>

	<xsl:template match="urg">
		<xsl:text>URG </xsl:text>
	</xsl:template>

	<xsl:template match="rst">
		<xsl:text>RST </xsl:text>
	</xsl:template>


	<xsl:template match="data/size">
		<div class="caption"><xsl:text>Payload size:</xsl:text></div>
		<div class="info"><xsl:value-of select="."/></div>
	</xsl:template>


	<xsl:template match="flags">
		<div class="caption"><xsl:text>Flags:</xsl:text></div>
		<div class="info"><xsl:text>[ </xsl:text><xsl:apply-templates select="*"/><xsl:text>]</xsl:text></div>
	</xsl:template>

	<xsl:template match="tcp">
		<xsl:param name="seq_start"/>
		<xsl:param name="ack_start"/>
		<xsl:apply-templates select="flags" />
		<div class="caption"><xsl:text>Seq/Ack nrs.:</xsl:text></div>
		<div class="info">
		<xsl:choose>
			<xsl:when test="seq - $seq_start">
				<xsl:text>+</xsl:text>
				<xsl:value-of select="seq - $seq_start"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="seq"/>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text> : </xsl:text>

		<xsl:choose>
			<xsl:when test="ack = 0">
				<xsl:text>0</xsl:text>
			</xsl:when>
			<xsl:when test="ack - $ack_start">
				<xsl:text>+</xsl:text>
				<xsl:value-of select="ack - $ack_start"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="ack"/>
			</xsl:otherwise>
		</xsl:choose>

		</div>
		<xsl:apply-templates select="data/size" />

	</xsl:template>

	<xsl:template match="packet">
		<xsl:param name="side1_start"/>
		<xsl:param name="side2_start"/>
		<xsl:choose>
			<xsl:when test="source = $side1">
				<div class="packet side1 {name(dropped)}{name(corrupted)}">
					<xsl:apply-templates select="tcp">
						<xsl:with-param name="seq_start" select="$side1_start"/>
						<xsl:with-param name="ack_start" select="$side2_start"/>
					</xsl:apply-templates>
				</div>
			</xsl:when>
			<xsl:when test="source = $side2">
				<div class="packet side2 {name(dropped)}{name(corrupted)}">
					<xsl:apply-templates select="tcp">
						<xsl:with-param name="seq_start" select="$side2_start"/>
						<xsl:with-param name="ack_start" select="$side1_start"/>
					</xsl:apply-templates>
				</div>
			</xsl:when>
		</xsl:choose>
	</xsl:template>


	<xsl:template match="log">
		<div class="log">
			<div class="header side1"><h2><xsl:value-of select="$side1"/></h2></div>
			<div class="header side2"><h2><xsl:value-of select="$side2"/></h2></div>
			<xsl:apply-templates select="packet">
				<xsl:with-param name="side1_start" select="packet[source = $side1]/tcp/seq/text()"/>
				<xsl:with-param name="side2_start" select="packet[source = $side2]/tcp/seq/text()"/>
				</xsl:apply-templates>
			<div class="float_end"></div>
		</div>
	</xsl:template>

</xsl:stylesheet>
