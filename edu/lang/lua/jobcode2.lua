console_dev = "/dev/ttyUSB4"
cmd="at+qsimstat=1"


function read_at(cmd)
    io.write(cmd.."\r\n")
    lines = ""
    line=""
    while not (string.find(line, "OK") or string.find(line, "ERROR")) do
        line = io.read()
        print(">> "..line)
        if (line ~= nil and line ~= "" and line ~= "\r") then
            lines = lines.." "..line
        end
    end
    lines = string.gsub(lines, "\r", "")
    return lines
end




os.execute("stty -F "..console_dev.." raw -echo -echoe -echok")
dev_file = io.open(console_dev, "a+")
io.input(dev_file)
io.output(dev_file)

--val1 = read_at("at+qsimstat=1")
print(val1)


io.close(dev_file)
