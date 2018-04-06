library(ggplot2)

data = read.csv("../strongscaling.csv")
str(data)

data$Fragments = as.factor(data$Fragments)

ggplot(data, aes(x = Processors, y = Time/60, color = Fragments)) + 
  geom_point(size=3, aes(shape = Fragments)) + geom_line(size = 1) + 
  scale_shape_manual(values = c(15, 19, 17, 8)) +
  scale_x_log10(breaks = c(128, 256, 512, 1024, 2048, 4096, 8192, 16384)) + 
  scale_y_continuous(breaks = c(seq(0, 70, by=10))) + 
  xlab("Number of Cores") + 
  ylab("Execution Time (Minutes)") + 
  #ggtitle("  Strong Scaling on Randomly Generated DNA Fragments") +  
  theme_bw()

ggsave("../../strong-scale.pdf", last_plot())