
import ROOT
import sys

ROOT.gROOT.ProcessLine(".L lifetimeCalc.cxx")
ROOT.gROOT.ProcessLine(".L plotAnodeCathodeDiff.cxx")

p = ROOT.plotAnodeCathodeDiff()

<<<<<<< HEAD
# p.RunPlotAndLifetime("Run_000349_00.txt")
# p.RunPlotAndLifetime("Run_000350_00.txt")
# p.RunPlotAndLifetime("Run_000351_00.txt")
# p.RunPlotAndLifetime("Run_000352_00.txt")
# p.RunPlotAndLifetime("Run_000353_00.txt")
# p.RunPlotAndLifetime("Run_000354_00.txt")
# p.RunPlotAndLifetime("Run_000355_00.txt")
# p.RunPlotAndLifetime("Run_000356_00.txt")
# p.RunPlotAndLifetime("Run_000357_00.txt")
# p.RunPlotAndLifetime("Run_000358_00.txt")
# p.RunPlotAndLifetime("Run_000359_00.txt")
# p.RunPlotAndLifetime("Run_000360_00.txt")
# p.RunPlotAndLifetime("Run_000361_00.txt")
# p.RunPlotAndLifetime("Run_000362_00.txt")
# p.RunPlotAndLifetime("Run_000363_00.txt")
# p.RunPlotAndLifetime("Run_000364_00.txt")
# p.RunPlotAndLifetime("Run_000365_00.txt")
# p.RunPlotAndLifetime("Run_000366_00.txt")
# p.RunPlotAndLifetime("Run_000367_00.txt")
# p.RunPlotAndLifetime("Run_000370_00.txt")
# p.RunPlotAndLifetime("Run_000371_00.txt")
# p.RunPlotAndLifetime("Run_000372_00.txt")
# p.RunPlotAndLifetime("Run_000373_00.txt")
# p.RunPlotAndLifetime("Run_000374_00.txt")
# p.RunPlotAndLifetime("Run_000375_00.txt")
# p.RunPlotAndLifetime("Run_000376_00.txt")
# p.RunPlotAndLifetime("Run_000377_00.txt")
# p.RunPlotAndLifetime("Run_000378_00.txt")
# p.RunPlotAndLifetime("Run_000379_00.txt")
# p.RunPlotAndLifetime("Run_000380_00.txt")
# p.RunPlotAndLifetime("Run_000381_00.txt")
# p.RunPlotAndLifetime("Run_000382_00.txt")
# p.RunPlotAndLifetime("Run_000383_00.txt")
# p.RunPlotAndLifetime("Run_000384_00.txt")
# p.RunPlotAndLifetime("Run_000385_00.txt")
# p.RunPlotAndLifetime("Run_000386_00.txt")
# p.RunPlotAndLifetime("Run_000387_00.txt")
# p.RunPlotAndLifetime("Run_000388_00.txt")
# p.RunPlotAndLifetime("Run_000389_00.txt")
# p.RunPlotAndLifetime("Run_000390_00.txt")
# p.RunPlotAndLifetime("Run_000391_00.txt")
# p.RunPlotAndLifetime("Run_000392_00.txt")
# p.RunPlotAndLifetime("Run_000393_00.txt")
# p.RunPlotAndLifetime("Run_000394_00.txt")
# p.RunPlotAndLifetime("Run_000395_00.txt")
# p.RunPlotAndLifetime("Run_000396_00.txt")
# p.RunPlotAndLifetime("Run_000397_00.txt")
# p.RunPlotAndLifetime("Run_000398_00.txt")
# p.RunPlotAndLifetime("Run_000399_00.txt")
# p.RunPlotAndLifetime("Run_000400_00.txt")
# p.RunPlotAndLifetime("Run_000401_00.txt")
# p.RunPlotAndLifetime("Run_000402_00.txt")
# p.RunPlotAndLifetime("Run_000403_00.txt")
# p.RunPlotAndLifetime("Run_000404_00.txt")
# p.RunPlotAndLifetime("Run_000405_00.txt")
# p.RunPlotAndLifetime("Run_000406_00.txt")
# p.RunPlotAndLifetime("Run_000407_00.txt")
# p.RunPlotAndLifetime("Run_000408_00.txt")
# p.RunPlotAndLifetime("Run_000409_00.txt")
# p.RunPlotAndLifetime("Run_000410_00.txt")
# p.RunPlotAndLifetime("Run_000411_00.txt")
# p.RunPlotAndLifetime("Run_000412_00.txt")
# p.RunPlotAndLifetime("Run_000413_00.txt")
# p.RunPlotAndLifetime("Run_000414_00.txt")
# p.RunPlotAndLifetime("Run_000415_00.txt")
# p.RunPlotAndLifetime("Run_000416_00.txt")
# p.RunPlotAndLifetime("Run_000417_00.txt")
# p.RunPlotAndLifetime("Run_000418_00.txt")
# p.RunPlotAndLifetime("Run_000419_00.txt")
# p.RunPlotAndLifetime("Run_000420_00.txt")
# p.RunPlotAndLifetime("Run_000421_00.txt")
# p.RunPlotAndLifetime("Run_000422_00.txt")
# p.RunPlotAndLifetime("Run_000423_00.txt")
# p.RunPlotAndLifetime("Run_000424_00.txt")
# p.RunPlotAndLifetime("Run_000425_00.txt")
# p.RunPlotAndLifetime("Run_000426_00.txt")
# p.RunPlotAndLifetime("Run_000427_00.txt")
# p.RunPlotAndLifetime("Run_000428_00.txt")
# p.RunPlotAndLifetime("Run_000429_00.txt")
# p.RunPlotAndLifetime("Run_000430_00.txt")
# p.RunPlotAndLifetime("Run_000431_00.txt")
# p.RunPlotAndLifetime("Run_000432_00.txt")
# p.RunPlotAndLifetime("Run_000433_00.txt")
# p.RunPlotAndLifetime("Run_000434_00.txt")
# p.RunPlotAndLifetime("Run_000435_00.txt")
# p.RunPlotAndLifetime("Run_000436_00.txt")
# p.RunPlotAndLifetime("Run_000437_00.txt")
# p.RunPlotAndLifetime("Run_000438_00.txt")
# p.RunPlotAndLifetime("Run_000439_00.txt")
# p.RunPlotAndLifetime("Run_000440_00.txt")
# p.RunPlotAndLifetime("Run_000441_00.txt")
# p.RunPlotAndLifetime("Run_000442_00.txt")
# p.RunPlotAndLifetime("Run_000443_00.txt")
# p.RunPlotAndLifetime("Run_000444_00.txt")
# p.RunPlotAndLifetime("Run_000445_00.txt")


# p.RunPlotAndLifetime("Run_000453_00.txt")
# p.RunPlotAndLifetime("Run_000454_00.txt")
# p.RunPlotAndLifetime("Run_000455_00.txt")
# p.RunPlotAndLifetime("Run_000456_00.txt")
# p.RunPlotAndLifetime("Run_000457_00.txt")
# p.RunPlotAndLifetime("Run_000460_00.txt")
# p.RunPlotAndLifetime("Run_000461_00.txt")
# p.RunPlotAndLifetime("Run_000462_00.txt")
# p.RunPlotAndLifetime("Run_000463_00.txt")
# p.RunPlotAndLifetime("Run_000464_00.txt")






p.RunPlotAndLifetime("Run_000719_01.txt")
p.RunPlotAndLifetime("Run_000720_01.txt")
p.RunPlotAndLifetime("Run_000721_01.txt")
p.RunPlotAndLifetime("Run_000722_01.txt")
p.RunPlotAndLifetime("Run_000723_01.txt")
p.RunPlotAndLifetime("Run_000724_01.txt")
p.RunPlotAndLifetime("Run_000725_01.txt")
p.RunPlotAndLifetime("Run_000726_01.txt")
p.RunPlotAndLifetime("Run_000727_01.txt")
p.RunPlotAndLifetime("Run_000728_01.txt")
p.RunPlotAndLifetime("Run_000729_01.txt")
p.RunPlotAndLifetime("Run_000730_01.txt")
p.RunPlotAndLifetime("Run_000731_01.txt")
p.RunPlotAndLifetime("Run_000732_01.txt")
p.RunPlotAndLifetime("Run_000733_01.txt")
p.RunPlotAndLifetime("Run_000734_01.txt")
p.RunPlotAndLifetime("Run_000735_01.txt")
p.RunPlotAndLifetime("Run_000736_01.txt")
p.RunPlotAndLifetime("Run_000737_01.txt")
p.RunPlotAndLifetime("Run_000738_01.txt")
p.RunPlotAndLifetime("Run_000739_01.txt")
p.RunPlotAndLifetime("Run_000740_01.txt")
p.RunPlotAndLifetime("Run_000741_01.txt")
p.RunPlotAndLifetime("Run_000742_01.txt")
p.RunPlotAndLifetime("Run_000743_01.txt")
p.RunPlotAndLifetime("Run_000744_01.txt")
p.RunPlotAndLifetime("Run_000745_01.txt")
p.RunPlotAndLifetime("Run_000746_01.txt")
p.RunPlotAndLifetime("Run_000747_01.txt")
p.RunPlotAndLifetime("Run_000748_01.txt")
p.RunPlotAndLifetime("Run_000749_01.txt")
p.RunPlotAndLifetime("Run_000750_01.txt")
p.RunPlotAndLifetime("Run_000751_01.txt")
p.RunPlotAndLifetime("Run_000752_01.txt")
p.RunPlotAndLifetime("Run_000753_01.txt")
p.RunPlotAndLifetime("Run_000754_01.txt")
p.RunPlotAndLifetime("Run_000755_01.txt")
p.RunPlotAndLifetime("Run_000756_01.txt")
p.RunPlotAndLifetime("Run_000757_01.txt")
p.RunPlotAndLifetime("Run_000758_01.txt")
p.RunPlotAndLifetime("Run_000759_01.txt")
p.RunPlotAndLifetime("Run_000760_01.txt")
p.RunPlotAndLifetime("Run_000761_01.txt")
p.RunPlotAndLifetime("Run_000762_01.txt")
p.RunPlotAndLifetime("Run_000763_01.txt")
p.RunPlotAndLifetime("Run_000764_01.txt")
p.RunPlotAndLifetime("Run_000765_01.txt")
p.RunPlotAndLifetime("Run_000766_01.txt")
p.RunPlotAndLifetime("Run_000767_01.txt")
p.RunPlotAndLifetime("Run_000768_01.txt")
p.RunPlotAndLifetime("Run_000769_01.txt")
p.RunPlotAndLifetime("Run_000770_01.txt")
p.RunPlotAndLifetime("Run_000771_01.txt")
p.RunPlotAndLifetime("Run_000772_01.txt")
p.RunPlotAndLifetime("Run_000773_01.txt")
p.RunPlotAndLifetime("Run_000774_01.txt")
p.RunPlotAndLifetime("Run_000775_01.txt")
p.RunPlotAndLifetime("Run_000777_01.txt")
p.RunPlotAndLifetime("Run_000778_01.txt")
p.RunPlotAndLifetime("Run_000779_01.txt")
p.RunPlotAndLifetime("Run_000780_01.txt")
p.RunPlotAndLifetime("Run_000781_01.txt")
p.RunPlotAndLifetime("Run_000782_01.txt")
p.RunPlotAndLifetime("Run_000783_01.txt")
p.RunPlotAndLifetime("Run_000784_01.txt")
p.RunPlotAndLifetime("Run_000785_01.txt")
p.RunPlotAndLifetime("Run_000786_01.txt")
p.RunPlotAndLifetime("Run_000787_01.txt")
p.RunPlotAndLifetime("Run_000788_01.txt")
p.RunPlotAndLifetime("Run_000789_01.txt")
p.RunPlotAndLifetime("Run_000790_01.txt")
p.RunPlotAndLifetime("Run_000791_01.txt")
p.RunPlotAndLifetime("Run_000793_01.txt")
p.RunPlotAndLifetime("Run_000794_01.txt")
p.RunPlotAndLifetime("Run_000795_01.txt")
p.RunPlotAndLifetime("Run_000796_01.txt")
p.RunPlotAndLifetime("Run_000797_01.txt")
p.RunPlotAndLifetime("Run_000798_01.txt")
p.RunPlotAndLifetime("Run_000799_01.txt")
p.RunPlotAndLifetime("Run_000800_01.txt")
p.RunPlotAndLifetime("Run_000801_01.txt")
p.RunPlotAndLifetime("Run_000802_01.txt")
p.RunPlotAndLifetime("Run_000803_01.txt")
p.RunPlotAndLifetime("Run_000804_01.txt")
p.RunPlotAndLifetime("Run_000805_01.txt")
p.RunPlotAndLifetime("Run_000806_01.txt")
p.RunPlotAndLifetime("Run_000807_01.txt")
p.RunPlotAndLifetime("Run_000808_01.txt")
p.RunPlotAndLifetime("Run_000809_01.txt")
p.RunPlotAndLifetime("Run_000810_01.txt")
p.RunPlotAndLifetime("Run_000811_01.txt")
p.RunPlotAndLifetime("Run_000812_01.txt")
p.RunPlotAndLifetime("Run_000813_01.txt")
p.RunPlotAndLifetime("Run_000814_01.txt")
p.RunPlotAndLifetime("Run_000815_01.txt")
p.RunPlotAndLifetime("Run_000816_01.txt")
p.RunPlotAndLifetime("Run_000817_01.txt")
p.RunPlotAndLifetime("Run_000818_01.txt")
p.RunPlotAndLifetime("Run_000819_01.txt")
p.RunPlotAndLifetime("Run_000820_01.txt")
p.RunPlotAndLifetime("Run_000821_01.txt")
p.RunPlotAndLifetime("Run_000822_01.txt")
p.RunPlotAndLifetime("Run_000823_01.txt")
p.RunPlotAndLifetime("Run_000824_01.txt")
p.RunPlotAndLifetime("Run_000825_01.txt")
p.RunPlotAndLifetime("Run_000826_01.txt")
p.RunPlotAndLifetime("Run_000827_01.txt")
p.RunPlotAndLifetime("Run_000828_01.txt")
p.RunPlotAndLifetime("Run_000829_01.txt")
p.RunPlotAndLifetime("Run_000830_01.txt")
p.RunPlotAndLifetime("Run_000831_01.txt")
p.RunPlotAndLifetime("Run_000832_01.txt")
p.RunPlotAndLifetime("Run_000833_01.txt")
p.RunPlotAndLifetime("Run_000834_01.txt")
p.RunPlotAndLifetime("Run_000835_01.txt")
p.RunPlotAndLifetime("Run_000836_01.txt")
p.RunPlotAndLifetime("Run_000837_01.txt")
p.RunPlotAndLifetime("Run_000838_01.txt")
p.RunPlotAndLifetime("Run_000839_01.txt")
p.RunPlotAndLifetime("Run_000830_01.txt")
p.RunPlotAndLifetime("Run_000831_01.txt")
p.RunPlotAndLifetime("Run_000832_01.txt")
p.RunPlotAndLifetime("Run_000833_01.txt")
p.RunPlotAndLifetime("Run_000834_01.txt")
p.RunPlotAndLifetime("Run_000835_01.txt")
p.RunPlotAndLifetime("Run_000836_01.txt")
p.RunPlotAndLifetime("Run_000837_01.txt")
p.RunPlotAndLifetime("Run_000838_01.txt")
p.RunPlotAndLifetime("Run_000839_01.txt")
=======
p.RunPlotAndLifetime("Run_000801_01.txt")

>>>>>>> 9a6f68a7aac8f1e01c388bcde7623aeacf937f32

