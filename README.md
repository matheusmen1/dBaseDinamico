# dBaseDinamico
<h4>dBase com listas encadeadas dinâmicas em C. A partir da linha de comando digitada pelo usuário, é interpretado e validado sua sintaxe e seus respectivos parâmetros. Sua interface se assemelha ao dBase original (década de 80, 90) e conta com suas principais funcionalidades: </h4>

1) SET DEFAULT TO:

2) CREATE : Cria um novo arquivo .DBF

3) DIR: para mostrar todos os arquivos .DBF criados na unidade atual.

4) QUIT: Encerra o ambiente do dBase.

5) USE: Abre um arquivo .DBF.

6) LIST STRUCTURE: Lista a estrutura criada para o arquivo .DBF.

7) APPEND: Insere novo registro ao final.

8) LIST: Lista todos os campos.

9) CLEAR: Limpa a tela do ambiente do dBase. (Não implementado)

10) LOCATE: Localiza registro(s), informando o campo e o conteúdo a ser buscado.

11) GOTO: Vai para determinado registro.

12) DISPLAY: Mostra o registro atual.

13) EDIT: Permite alterar os dados do registro atual.

14) DELETE: Marca o registro atual para exclusão (um tipo de exclusão lógica).
    DELETE ALL {este comando marca todos os registros para exclusão} 

15) RECALL: Reativa o(s) registro(s) marcado(s) para exclusão.
    RECALL ALL

16) SET DELETED

    SET DELETED OFF {Este comando permite que se possa visualizar e ou reativar registros marcados para exclusão}

    SET DELETED ON {Este comando esconde os registros marcados para exclusão}

18) PACK: Exclui fisicamente os registros marcados para exclusão.

19) ZAP: Este comando exclui todos os registros do arquivo .DBF em uso.

20) MODIFY STRUCTURE: Permite modificar a estrutura do arquivo .DBF (alterar os campos existentes ou criar novos).

21) SORT: Ordena os dados em relação a algum campo especificado.

<h4>Exemplos de algumas funcionalidades:</h4>

<h3>Menu Principal</h3>


![Menu Principal](imagens/menuPrincipal.png)

<h3>Create</h3>


![Create](imagens/create.png)


<h3>Dir</h3>


![Dir](imagens/dir.png)


<h3>List Structure</h3>


![List Structure](imagens/listStructure.png)


<h3>Append</h3>


![Append](imagens/append.png)


<h3>List</h3>


![List](imagens/list.png)


<h3>List For</h3>


![List For](imagens/listfor.png)


<h3>Sort</h3>


![Sort](imagens/sort.png)
![Sort2](imagens/sort2.png)
