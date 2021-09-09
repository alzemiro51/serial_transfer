# serial_transfer

Aplicação cliente-servidor para envio de arquivos via serial para Windows

O cliente verifica a pasta c:\temp\cliente\files e envia os arquivos encontrados para o servidor

O servidor salva os arquivos na pasta onde a aplicação é executada

O cliente se conecta na porta COM3 e o servidor na porta COM4, caso seja necessário alterar as portas, o código fonte deve ser modificado

Os projetos podem ser abertos e compilados no CodeBlocks para Windows
