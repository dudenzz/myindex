import gensim
#import codecs
import sys
import zmq
import re


try:
    #clinPath = u'L:/Python_projects/TREC_2019/VSM/2018_notlower_clintr_400_window20_min_count1.bin'
    clinPath = u'L:/Python_projects/TREC_2019/VSM/2018_clintr_simplepreprocess_lower_kit_genekit_400_window20_min_count1.bin'
    modelClin = gensim.models.KeyedVectors.load_word2vec_format(clinPath, binary=True)
    thresholdClin = 0.5
except (Exception, IndexError) as e:
    print repr(e) + "\t" + format(
        sys.exc_info()[-1].tb_lineno) + '\n'
    raise SystemExit

print "VSM loaded"

port = "12000"
context = zmq.Context()
socket = context.socket(zmq.PAIR)
socket.bind("tcp://*:%s" % port)

while True:
    terms = socket.recv_unicode()
    check = terms.split(u'# ')

    if check[0]==u'__SET__THRESHOLD__':
        thresholdClin = float(check[1])
        print 'Instruction received:'
        print check[0]
        print 'Threshold set to ' + str(thresholdClin)
        continue
    if check[0] == u'__SET__VSM__':
        if check[1]==clinPath:
            print check[1]+' already set'
            continue
        try:
            clinPath = check[1]
            print clinPath
            modelClin = gensim.models.KeyedVectors.load_word2vec_format(clinPath, binary=True)
        except (Exception, IndexError) as e:
            print repr(e) + "\t" + format(
                sys.exc_info()[-1].tb_lineno) + '\n'
            raise SystemExit
        print 'Instruction received:'
        print check[0]
        print 'VSM set to ' + check[1]
        continue

    terms = terms.split()
    qexpClin = []
    try:
        qexpClin = modelClin.most_similar(terms)
    except Exception as e:
        print e.message
        print e.args
        for arg in e.args:
            m = re.search("'(.+?)'",arg)
        if m:
            toRemove = m.group(1)
            print toRemove
            terms.remove(toRemove)
            try:
                qexpClin = modelClin.most_similar(terms)
            except Exception as e:
                print e.message

    #print qexpClin
    toSend=''
    expterms=[]
    for term in qexpClin:
        score = float(term[1])
        if score >= thresholdClin:
            expterms.append(term[0])
    #print expterms

    for term in expterms:
        toSend+=term+' '
    socket.send_unicode(toSend)

