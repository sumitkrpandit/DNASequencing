library(ggplot2)

data = read.csv("weakscaling.csv")

ggplot(data, aes(x = Processors, y = Time)) + 
  geom_point(size=2) + geom_line(size = 1) + 
  #scale_fill_manual(breaks = c(64, 128, 256, 512, 1024, 2048, 4096, 8192), labels = c("64", "128", "256", "512", "1024", "2048", "4096", "8192")) + 
  scale_x_log10(breaks = c(64, 128, 256, 512, 1024, 2048, 4096, 8192), labels = c("(64, 1)", "(128, 2)", "(256, 4)", "(512, 8)", "(1024, 16)", "(2048, 32)", "(4096, 64)", "(8192, 128)")) + 
  xlab("(Processors, Nodes)") + 
  ylab("Execution Time (Seconds)") + 
  ggtitle("                     Weak Scaling on Randomly Generated DNA Fragments") +  
  theme_bw()
