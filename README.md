# hyperseqqqq<<<

hypersonix sequencer focused on performance <<<

next evolution state of knitt rider sequencer by bastl <<< (i hope)

based on raspberry pico

## Motivation

I want to build eurorack drummachine (with softpop as monovoice <<) and I need a sequencer. I currently use po-33 as drum voice and I really like the interface as it is really performative, I just miss some things -- I want to build the drums and not use just samples and need some more performance tricks.

## Concept

since raspberry pico has only 24 digital io and 3 analog io and my eurorack will have limited space I've decided to try putting it into 6 hp with only 8 steps (its also enough when you are programming rhytms on the fly). You select one of the 6 channels using dedicated button. 

But there are also other tricks.

I mostly build drums on the fly while performing so paterns are not so important for me, but in some cases you need them, so there will be 8 of them using shift function with chaining.

But what could dramaticaly increase creativity with same performing power would be clock dividers on each channel (1, 2, 3, 4, 5, 8) allowing complex rhytms and polyrhytms

In order to allow polymeters (alongside polyrhytms) it will also have active steps

And most performative thing is what I call "freeze mode." You basicaly freeze the channel so it keeps playing the same rhytm while you can program new one, once you unfreeze it, new beat will start playing. This allows you to have more time building your patterns hyeeeeeer

## Implementation

firstly I wanted to know if the interface makes sence, so I've made prototype in VCV Rack

TO BE DONE
