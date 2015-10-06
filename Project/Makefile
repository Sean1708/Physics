SOURCE=$(wildcard *.tex *.ltx *.latex)
ifeq ($(origin TARGET), undefined)
	# if TARGET is expanded at use it will include .bib files, this is BAD!!!!!1!11!!
	EXPANDED_SOURCE:=$(SOURCE)
	TARGET+=$(subst .tex,.pdf,$(filter %.tex,$(EXPANDED_SOURCE)))
	TARGET+=$(subst .ltx,.pdf,$(filter %.ltx,$(EXPANDED_SOURCE)))
	TARGET+=$(subst .latex,.pdf,$(filter %.latex,$(EXPANDED_SOURCE)))
endif
SOURCE+=$(wildcard *.bib)

MEDIA_TO_CONVERT=$(filter-out %.pdf,$(wildcard media/*))
CONVERTED_MEDIA=$(addsuffix .pdf,$(notdir $(basename $(MEDIA_TO_CONVERT))))
MEDIA_TO_COPY=$(filter %.pdf,$(wildcard media/*))
COPIED_MEDIA=$(notdir $(MEDIA_TO_COPY))

all: $(TARGET)

$(TARGET): $(CONVERTED_MEDIA) $(COPIED_MEDIA) $(SOURCE)
	latexmk

# don't fucking ask, just look here:
#     http://stackoverflow.com/a/9647428
# basically for every (MEDIA_TO_CONVERT, CONVERTED_MEDIA) pair it produces
# the output `convert MEDIA_TO_CONVERT CONVERTED_MEDIA`
$(CONVERTED_MEDIA): $(MEDIA_TO_CONVERT)
	$(subst ^, ,$(join $(addprefix convert^,$(MEDIA_TO_CONVERT)),$(patsubst %,^%;,$(CONVERTED_MEDIA))))

$(COPIED_MEDIA): $(MEDIA_TO_COPY)
	cp $(MEDIA_TO_COPY) $(dir MEDIA_TO_COPY)

clean:
	latexmk -c

cleanall: clean
	rm -f $(TARGET) $(CONVERTED_MEDIA) $(COPIED_MEDIA)
