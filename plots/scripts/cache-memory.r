library(ggplot2)

data = read.csv("../memory.csv")

data$Size = as.factor(data$Size)

ggplot(data, aes(x = Size, y = Memory, fill = Implementation)) + 
  geom_bar(width = 0.8, stat = "identity", position = position_dodge()) + 
  scale_fill_brewer(palette = "Set1") + 
  #ggtitle("                                 Cache Memory Footprint") + 
  xlab("DNA Fragments (~125 bps)") + 
  ylab("Memory Usage (GBs)") +
  theme_bw()
