/**
 * Project: Phoenix
 * Time-stamp: <2025-02-28 00:57:54 doug>
 *
 * @file ConfigureXslt.cpp
 * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * @author Douglas Mark Royer
 * @date 24-FEB-20205
 *
 * licensed under CC BY 4.0.
 *
 * RiverExplorer is a trademark of Douglas Mark Royer
 */

namespace RiverExplorer::Phoenix
{
	// This looks silly to have this here.
	// But G++ declares the variabe local and I can't link to it,
	// until I added this to this file.
	//
	extern const char * const ConfigToHtml;

	/**
	 * This XSLT stylesheet converts the configuration XML
	 * file to HTML.
	 */
	const char * const ConfigToHtml = R"(<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" 
								xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
								xmlns:re="http://RiverExplorer.US/Namespaces/XML">
  <xsl:template match="/">
    <html>
      <head>
        <title>RiverExplorer Configuration</title>
        <style>
          .border { border-collapse: collapse; width: 100%; margin: 20px 0; }
          .no-border { border-collapse: collapse; border: none;}
          th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }
          th { background-color: #f2f2f2; }
          .summary { font-style: italic; color: #666; }
          .detail { font-size: 0.9em; }
          h1 { color: #333; }
        </style>
      </head>
      <body>
				<table class="border" with="100%">
					<tr class="no-border">
						<td class="no-border">
							<img width="100" height="100" src="RiverExplorerLogo-1-4096x2304-Transparent-2.png"/>
						</td>
						<td class="no-border" style="align-text:right">
							<h1>RiverExplorer Phoenix Configuration</h1>
						</td>
					</tr>
				</table>
				<hr/>
				<ul>
					<xsl:for-each select="//re:Key">
						<li>
							<xsl:value-of select="re:Detail"/>
							<table>
								<tr>
									<th width="10%">Key</th>
									<xsl:choose>
										<xsl:when test="re:Value/@ReadOnly">
											<th width="25%">Value (Read-Only)</th>
										</xsl:when>
										<xsl:otherwise>
											<th width="25%">Value</th>
										</xsl:otherwise>
									</xsl:choose>
									<th>Summary</th>
								</tr>
								<tr>
									<td><xsl:value-of select="re:Name"/></td>
									<td><xsl:value-of select="re:Value"/></td>
									<td class="summary"><xsl:value-of select="re:Summary"/></td>
								</tr>
							</table>	
							<hr/>
					</li>
					</xsl:for-each>
				</ul>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>
)";
}
