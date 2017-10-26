    mov ax,0xb800
    mov es,ax

    mov byte [es:0x00],'L'
    mov byte [es:0x01],0x07
    mov byte [es:0x02],'a'
    mov byte [es:0x03],0x07
    mov byte [es:0x04],'b'
    mov byte [es:0x05],0x07
    mov byte [es:0x06],'e'
    mov byte [es:0x07],0x07
    mov byte [es:0x08],'l'
    mov byte [es:0x09],0x07
    mov byte [es:0x0a],' '
    mov byte [es:0x0b],0x07
    mov byte [es:0x0c],'o'
    mov byte [es:0x0d],0x07
    mov byte [es:0x0e],'f'
    mov byte [es:0x0f],0x07
    mov byte [es:0x10],'f'
    mov byte [es:0x11],0x07
    mov byte [es:0x12],'s'
    mov byte [es:0x13],0x07
    mov byte [es:0x14],'e'
    mov byte [es:0x15],0x07
    mov byte [es:0x16],'t'
    mov byte [es:0x17],0x07
    mov byte [es:0x18],':'
    mov byte [es:0x19],0x07

    mov ax,number
    mov bx,10

    mov cx,cs
    mov ds,cx

    mov dx,0
    div bx
    mov [0x7c00+number+0x00],dl

    xor dx,dx
    div bx
    mov [0x7c00+number+0x01],dl

    xor dx,dx
    div bx
    mov [0x7c00+number+0x02],dl

    xor dx,dx
    div bx
    mov [0x7c00+number+0x03],dl
    
    xor dx,dx
    div bx
    mov [0x7c00+number+0x04],dl

    mov al,[0x7c00+number+0x04]
    add al,0x30
    mov [es:0x1a],al
    mov byte [es:0x1b],0x04

    mov al,[0x7c00+number+0x03]
    add al,0x30
    mov [es:0x1c],al
    mov byte [es:0x1d],0x04

    mov al,[0x7c00+number+0x02]
    add al,0x30
    mov [es:0x1e],al
    mov byte [es:0x1f],0x04

    mov al,[0x7c00+number+0x01]
    add al,0x30
    mov [es:0x20],al
    mov byte [es:0x21],0x04

    mov al,[0x7c00+number+0x00]
    add al,0x30
    mov [es:0x22],al
    mov byte [es:0x23],0x04

    mov byte [es:0x24],'D'
    mov byte [es:0x25],0x07

infi:
    jmp near infi
    number db 0,0,0,0,0
    
    times 203 db 0
              db 0x55,0xaa

