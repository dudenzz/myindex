import xml.etree.ElementTree as ET
from stop_words import get_stop_words

gFile = open ('gene_synonyms_case.txt')
gSyns = []
for line in gFile:
    line = line.split('\t')
    for l in line:
        l = l.replace('\n','')
        gSyns.append(l)
gFile.close()

topicspath = 'M:\\trec\\2018\\topics2018.xml'
tree = ET.parse(topicspath)
root = tree.getroot()

disFile = open ('qDiseases.txt','w')
geneFile = open ('qGenes.txt','w')
varFile = open ('qVariants.txt','w')

elements = root.findall('topic')
for iter, element in enumerate(elements):
    docno = str(iter + 1) + ';;'
    disease = element.find("disease").text.lower().replace('\n', '')
    gene = element.find("gene").text.lower().replace('\n', '')
    gene = gene.split(',')
    genes = ''
    variants = ''
    for g in gene:
        g = g.split()
        for entry in g:
            if entry.lower() in gSyns and entry.lower()!='of' and entry.lower()!='for':
                genes += entry.lower()
            else:
                variants += entry.lower() + ' '
        if len(gene)>1:
            genes += ','
            variants += ','

    disFile.write(docno+disease+'\n')
    geneFile.write(docno+genes+'\n')
    varFile.write(docno+variants+'\n')
disFile.close()
geneFile.close()
varFile.close()



