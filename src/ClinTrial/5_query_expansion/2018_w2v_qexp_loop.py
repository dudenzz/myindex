#import gensim
import codecs
import sys
import os
import zmq
from copy import deepcopy

WeightOrigStart = 120
WeightOrigStop = 190
WeightOrigStep = 20
WeightExpStart=1
WeightExpStop=2
WeightExpStep=1

"""VPaths = ['__SET__VSM__# L:/Python_projects/TREC_2019/VSM/2018_notlower_clintr_400_window20_min_count1.bin',
          '__SET__VSM__# M:/biocaddie data/bio_nlp/bio_nlp_vec.tar/bio_nlp_vec/PubMed-shuffle-win-30.bin']"""

"""VPaths = ['__SET__VSM__# L:/Python_projects/TREC_2019/VSM/2018_clintr_simplepreprocess_lower_kit_genekit_400_window20_min_count1.bin',
          '__SET__VSM__# M:/biocaddie data/bio_nlp/bio_nlp_vec.tar/bio_nlp_vec/PubMed-shuffle-win-30.bin']"""

VPaths = ['__SET__VSM__# L:/Python_projects/TREC_2019/VSM/2018_clintr_simplepreprocess_lower_kit_genekit_400_window20_min_count1.bin']


#thresholds = [0.50,0.55,0.60,0.65,0.70,0.75,0.80,0.85]

thresholds = [0.55,0.60]


disFile = open ('qDiseases.txt')
geneFile = open ('qGenes.txt')
varFile = open ('qVariants.txt')

pathQueries = 'M:\\terrier\\trec_2017\\terrier-core-4.2\\var\\queries_2018\\'





port = "12000"
context = zmq.Context()
socket = context.socket(zmq.PAIR)
socket.connect("tcp://localhost:%s" % port)

keys = ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '26', '27', '28', '29', '30', '31', '32', '33', '34', '35', '36', '37', '38', '39', '40', '41', '42', '43', '44', '45', '46', '47', '48', '49', '50']

qterms = {}
expandedTerms = {}
origTerms = {}
#keys = []

#print qterms
for line in disFile:
    line = line.split(';;')
    #keys.append(line[0])
    # queries[line[0]] = line[0] + ' ' + line[1].replace('\n',' ')
    qterms[line[0]] = []
    expandedTerms[line[0]] = []
    origTerms[line[0]] = []
    disTerms = line[1].split()
    for dTerm in disTerms:
        qterms[line[0]].append(dTerm)
        expandedTerms[line[0]].append(dTerm.lower())
        origTerms[line[0]].append(dTerm.lower())

#print qterms
for line in geneFile:
    line = line.split(';;')
    key = line[0]
    genes = line[1].replace('\n', '').split(',')
    for gene in genes:
        # queries[key]+= gene.upper() + ' '
        if gene != '':
            #print key

            #qterms[key].append(gene.upper())
            if gene=='kit':
                gene = "gene_kit"

            qterms[key].append(gene)

            expandedTerms[key].append(gene)
            origTerms[key].append(gene)

for line in varFile:
    line = line.split(';;')
    key = line[0]
    variants = line[1].replace('\n', '').split(',')
    for variant in variants:
        variant = variant.split()
        for v in variant:
            if v.startswith('('):
                v = v.replace('(', '')
                v = v.replace(')', '')
                #v = v.upper()
            qterms[line[0]].append(v)
            expandedTerms[line[0]].append(v.lower())
            origTerms[line[0]].append(v.lower())

#print qterms
#print expandedTerms
#print origTerms

#os.system("pause")

for VPath in VPaths:
    socket.send_unicode(VPath)
    print 'VPath='+VPath
    for threshold in thresholds:
        print 'threshold='+str(threshold)
        newThr = '__SET__THRESHOLD__# '+str(threshold)
        socket.send_unicode(newThr)



        results = {}
        #print qterms
        #print keys
        for key in keys:
            # print qterms[key]

            toSend = u''
            for term in qterms[key]:
                toSend += term + ' '
            socket.send_unicode(toSend)
            result = socket.recv_unicode()
            result = result.split()
            results[key] = result

        #print results
        #os.system("pause")

        for weightOrig in range(WeightOrigStart,WeightOrigStop,WeightOrigStep):
            print 'WeightOrig='+str(weightOrig)
            for weightExpand in range(WeightExpStart,WeightExpStop,WeightExpStep):


                newQuery = ''

                index=1
                newExpandedTerms = deepcopy(expandedTerms)

                for qnumb in range(1,len(keys)+1):
                    for r in results[str(qnumb)]:
                        newExpandedTerms[str(qnumb)].append(r.lower())
                    #print expandedTerms[key]
                        newExpandedTerms[str(qnumb)] = list(dict.fromkeys(newExpandedTerms[str(qnumb)]))
                    #print expandedTerms[key]
                    newQuery += str(index)+' '
                    for term in newExpandedTerms[str(qnumb)]:
                        if u'.'in term:
                            continue
                        if len(newExpandedTerms[str(qnumb)])==len(origTerms[str(qnumb)]):
                            newQuery += term + ' '
                        else:

                            if term in origTerms[str(qnumb)]:
                                newQuery += term +'^'+str(weightOrig)+ ' '
                            else:
                                newQuery += term + '^' + str(weightExpand) + ' '
                    newQuery += '\n'
                    index+=1

                #print newQuery
                #os.system("pause")
                vectors = VPath.split('/')

                qFileName = pathQueries+vectors[len(vectors)-1]+'_'+str(threshold)+'_wo_'+str(weightOrig)+'_we_'+str(weightExpand)+'.txt'

                qFile = open (qFileName,'w')
                qFile.write(newQuery.encode('utf-8'))
                qFile.close()
disFile.close()
geneFile.close()
varFile.close()