- Introdução

Em alguns casos precisamos que sistemas desenvolvidos em linguagem e plataformas diferentes se comuniquem via internet para diversos fins como, integração, adaptação, reutilização e etc. Essa necessidade é mais frequente entre empresas que realizam algum tipo de parceria e necessitam trocar informações. Para resolver este problema, os sistemas irão se comunicar utilizando uma linguagem universal conhecida por eles, chamamos essa tecnologia de WebServices.

- Problema

	Em nosso problema, temos que a universidade UFRRJ está aceitando inscrições para o seu programa de Pós-Graduação. Para tanto, os candidatos que quiserem concorrer a uma vaga devem realizar a inscrição submetendo seus boletins escolares para o sistema da UFRRJ em formato XML que deverá respeitar um padrão proposto.
	

- Solução
	
	O sistema da UFRRJ disponibilizará dois métodos para serem invocados. O primeiro será o “submeter” que receberá um boletim como parâmetro e retornará um número inteiro como resultado da inscrição, sendo 0(sucesso), 1(XML inválido), 2(XML mal-formado) e 3(Erro interno). O segundo será o “consultaStatus”, que receberá um CPF como parâmetro e retornará um número inteiro como resultado da consulta, sendo 0(candidato não encontrado), 1(Em processamento), 2(Candidato aprovado e selecionado), 3(Candidato aprovado e em espera) e 4(Candidato não aprovado).
	O sistema precisa receber as invocações de métodos e os parâmetros respeitando um formato. Para isso será utilizado o XML Schema para validar os formatos, se necessário retornando erros especificados anteriormente.
	Nos casos onde os boletins vierem em formatos diferentes do proposto, o sistema tentará aplicar uma transformação a fim de ficar no formato proposto, para isso será utilizado a tecnologia XSLT.
