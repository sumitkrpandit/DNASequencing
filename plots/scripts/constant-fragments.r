require(ggplot2)

plot <- ggplot(data = data.frame(x=0), mapping = aes(x=x))  #dummy data set

fun.1 <- function(x) ((x + 4)*10000^2 - 4 * 10000)/1024^3   #old

fun.2 <- function(x) (8*10000^2 + (x-4) * 10000)/1024^3   #new

plot + stat_function(fun = fun.1, size = 1, aes(colour = "REGULAR"))  + 
  stat_function(fun = fun.2, size = 1, aes(colour = "INDEXED")) + 
  scale_x_continuous(limits = c(1, 500)) +
  #scale_y_log10(breaks = c(seq(0,1000, by = 50), seq(1000, 50000, by = 5000))) +  
  scale_color_manual(name = "Implementation", values = c("red", "blue")) +
  theme_bw() +
  xlab("Basepairs (10000 DNA Fragments)") +
  ylab("Memory Usage (GBs)")
  #ggtitle("   Theoretical Memory Footprint (n = 10000 fragments)")

