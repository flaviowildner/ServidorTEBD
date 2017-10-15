<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/universidade/documento">
<boletim xmlns="https://www.w3schools.com">
  <informacao_pessoal>
    <nome>
      <xsl:value-of select="dados_pessoais/nome"/>
    </nome>
    <CPF>
    	<xsl:value-of select="dados_pessoais/cpf"/>
    </CPF>
    <matricula>
      <xsl:value-of select="substring(dados_pessoais/registro_universitario,3)"/>
    </matricula>
    <endereco>
    	<xsl:value-of select="dados_pessoais/endereco/tipo_logradouro"/><xsl:text> </xsl:text><xsl:value-of select="dados_pessoais/endereco/logradouro"/><xsl:text> </xsl:text><xsl:value-of select="dados_pessoais/endereco/numero"/>
    </endereco>
    <telefone>
      <xsl:if test="dados_pessoais/contatos/contato/@tipo='telefone residencial'">
        <xsl:value-of select="dados_pessoais/contatos/contato[@tipo='telefone residencial']/@valor" />
      </xsl:if>
    </telefone>
  </informacao_pessoal>
  
	<xsl:for-each select="periodos/periodo">
    <xsl:if test="not(@situacao='trancado')">
    <informacao_periodo>
        <data>
          <xsl:value-of select="@ano"/>
          <xsl:if test="@semestre='1'">-01-01
        	</xsl:if>
   				<xsl:if test="@semestre='2'">-06-01
   				</xsl:if></data>
        <cr_periodo>
          <xsl:value-of select="round(@cr)"/>
        </cr_periodo>
      
      <xsl:for-each select="aprovado">
        <materia>
          <nome>
            <xsl:value-of select="@nome"/>
          </nome>
          <nota_final>
            <xsl:value-of select="@nota"/>
          </nota_final>
          <carga_horaria>
            <xsl:value-of select="@carga_horaria"/>
          </carga_horaria>
          <creditos>
            <xsl:value-of select="@creditos"/>
          </creditos>
          <situacao_final>
            <xsl:text>Aprovado</xsl:text>
          </situacao_final>
        </materia>
      </xsl:for-each>

     <xsl:for-each select="reprovado">
        <materia>
          <nome>
            <xsl:value-of select="@nome"/>
          </nome>
          <nota_final>
            <xsl:value-of select="@nota"/>
          </nota_final>
          <carga_horaria>
            <xsl:value-of select="@carga_horaria"/>
          </carga_horaria>
          <creditos>
            <xsl:value-of select="@creditos"/>
          </creditos>
          <situacao_final>
            <xsl:text>Reprovado</xsl:text>
          </situacao_final>
        </materia>
      </xsl:for-each>

     <xsl:for-each select="abandono">
        <materia>
          <nome>
            <xsl:value-of select="@nome"/>
          </nome>
          <nota_final>
            <xsl:value-of select="@nota"/>
          </nota_final>
          <carga_horaria>
            <xsl:value-of select="@carga_horaria"/>
          </carga_horaria>
          <creditos>
            <xsl:value-of select="@creditos"/>
          </creditos>
          <situacao_final>
            <xsl:text>Abandono</xsl:text>
          </situacao_final>
        </materia>
      </xsl:for-each>

    </informacao_periodo>
   </xsl:if>
	</xsl:for-each>

</boletim>

</xsl:template>
</xsl:stylesheet>