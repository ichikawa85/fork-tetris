# coding: utf-8

num = ARGV[0]

ps = `ps aux`
str = []
cols = Array.new
pids = Array.new
count = 0
ps.each_line do |line|
  if line.include?("tetris")
    str = line
    cols = str.chomp.split(/\s+/)
    pids.push(cols[1])
    count = count + 1
  end
end

p pids[num.to_i].to_i
