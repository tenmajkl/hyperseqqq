#include "plugin.hpp"


struct Seq : Module {
	enum ParamId {
		BUTTON_1_PARAM,
		BUTTON_2_PARAM,
		BUTTON_3_PARAM,
		BUTTON_4_PARAM,
		BUTTON_5_PARAM,
		BUTTON_6_PARAM,
		BUTTON_7_PARAM,
		BUTTON_8_PARAM,
		BUTTON_9_PARAM,
		BUTTON_10_PARAM,
		BUTTON_11_PARAM,
		BUTTON_12_PARAM,
		BUTTON_13_PARAM,
		BUTTON_14_PARAM,
		BUTTON_15_PARAM,
		BUTTON_16_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		CLOCK_IN_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUT_1_OUTPUT,
		OUT_2_OUTPUT,
		OUT_3_OUTPUT,
		OUT_4_OUTPUT,
		OUT_5_OUTPUT,
		OUT_6_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHT_1_LIGHT,
		LIGHT_2_LIGHT,
		LIGHT_3_LIGHT,
		LIGHT_4_LIGHT,
		LIGHT_5_LIGHT,
		LIGHT_6_LIGHT,
		LIGHT_7_LIGHT,
		LIGHT_8_LIGHT,
		LIGHT_9_LIGHT,
		LIGHT_10_LIGHT,
		LIGHT_11_LIGHT,
		LIGHT_12_LIGHT,
		LIGHT_13_LIGHT,
		LIGHT_14_LIGHT,
		LIGHT_15_LIGHT,
		LIGHT_16_LIGHT,
		LIGHTS_LEN
	};

    enum Mode {
        DEFAULT,
        DIVIDE,
        ACTIVE
    };

    enum ParamId selected = BUTTON_1_PARAM;

    bool sequence[6][8];

    bool high[8];

    bool gate_high = false;

    int counter = 0;

    int trigger_len = 0;

    enum Mode mode = DEFAULT;

    bool divide_high = false;
    bool active_high = false;

    

	Seq() {
//        lights[selected].setBrightness(100);
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 8; j++) {
                sequence[i][j] = false;
            }
        }

        for (int i = 0; i < 8; i++) {
            high[i] = false;
        }

		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configButton(BUTTON_1_PARAM, "Push");
		configButton(BUTTON_2_PARAM, "Push");
		configButton(BUTTON_3_PARAM, "Push");
		configButton(BUTTON_4_PARAM, "Push");
		configButton(BUTTON_5_PARAM, "Push");
		configButton(BUTTON_6_PARAM, "Push");
		configButton(BUTTON_7_PARAM, "Push");
		configButton(BUTTON_8_PARAM, "Push");
		configButton(BUTTON_9_PARAM, "Push");
		configButton(BUTTON_10_PARAM, "Push");
		configButton(BUTTON_11_PARAM, "Push");
		configButton(BUTTON_12_PARAM, "Push");
		configButton(BUTTON_13_PARAM, "Push");
		configButton(BUTTON_14_PARAM, "Push");
		configButton(BUTTON_15_PARAM, "Push");
		configButton(BUTTON_16_PARAM, "Push");
		configInput(CLOCK_IN_INPUT, "");
		configOutput(OUT_1_OUTPUT, "");
		configOutput(OUT_2_OUTPUT, "");
		configOutput(OUT_3_OUTPUT, "");
		configOutput(OUT_4_OUTPUT, "");
		configOutput(OUT_5_OUTPUT, "");
		configOutput(OUT_6_OUTPUT, "");
	}


	void process(const ProcessArgs& args) override {
        lights[selected].setBrightness(100);
        
        if (mode != DEFAULT) {
            // dirty hack lmao welcome to c swaggers
            bool m = mode - 1;
            lights[6 + m].setBrightness(100);
            lights[6 + !m].setBrightness(0);
        } else {
            lights[6].setBrightness(0);
            lights[7].setBrightness(0);
        }

        if (inputs[CLOCK_IN_INPUT].getVoltage() > 0 && !gate_high) {
            gate_high = true;
            for (int i = 0; i < 6; i++) {
                outputs[i].setVoltage(sequence[i][counter] * 10);
            }
            
            if (mode == DEFAULT) {
                int prev = counter == 0 ? 7 : counter - 1;
                lights[8 + prev].setBrightness(sequence[selected][prev] * 100);
                lights[8 + counter].setBrightness(!sequence[selected][counter] * 100);
            }

            counter++;
            if (counter == 8) {
                counter = 0;
            }
        }

        if (inputs[CLOCK_IN_INPUT].getVoltage() == 0 && gate_high) {
            gate_high = false;
        }

        trigger_len++;
        if (trigger_len == 1024) {
            trigger_len = 0;
            for (int i = 0; i < 6; i++) {
                outputs[i].setVoltage(0);
            }
        }

        if (params[BUTTON_7_PARAM].getValue() > 0 && !divide_high) {
            divide_high = true;
            mode = mode == DIVIDE ? DEFAULT : DIVIDE;
        }

        if (params[BUTTON_8_PARAM].getValue() > 0 && !active_high) {
            active_high = true;
            mode = mode == ACTIVE ? ACTIVE : ACTIVE;
        }

        if (params[BUTTON_7_PARAM].getValue() == 0 && divide_high) {
            divide_high = false;
        }

        if (params[BUTTON_8_PARAM].getValue() == 0 && active_high) {
            active_high = false;
        }

	    for (int i = 0; i < 6; i++) {
            if (params[i].getValue() > 0) {
                lights[selected].setBrightness(0);
                selected = static_cast<ParamId>(i);
                for (int j = 0; j < 8; j++) {
                    lights[8 + j].setBrightness(sequence[selected][j] * 100);
                }
            }
        }

        for (int i = 8; i < 16; i++) {
            if (params[i].getValue() > 0.f && !high[i - 8]) {
                high[i - 8] = true;
                sequence[selected][i - 8] = !sequence[selected][i - 8];
                lights[i].setBrightness(sequence[selected][i - 8] * 100);
            }

            if (params[i].getValue() == 0 && high[i - 8]) {
                high[i - 8] = false;
            }
        }
    }
};


struct SeqWidget : ModuleWidget {
	SeqWidget(Seq* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/seq.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<VCVButton>(mm2px(Vec(13.835, 22.25)), module, Seq::BUTTON_1_PARAM));
		addParam(createParamCentered<VCVButton>(mm2px(Vec(25.035, 22.25)), module, Seq::BUTTON_2_PARAM));
		addParam(createParamCentered<VCVButton>(mm2px(Vec(13.835, 34.25)), module, Seq::BUTTON_3_PARAM));
		addParam(createParamCentered<VCVButton>(mm2px(Vec(25.035, 34.25)), module, Seq::BUTTON_4_PARAM));
		addParam(createParamCentered<VCVButton>(mm2px(Vec(13.835, 46.25)), module, Seq::BUTTON_5_PARAM));
		addParam(createParamCentered<VCVButton>(mm2px(Vec(25.035, 46.25)), module, Seq::BUTTON_6_PARAM));
		addParam(createParamCentered<VCVButton>(mm2px(Vec(13.835, 58.25)), module, Seq::BUTTON_7_PARAM));
		addParam(createParamCentered<VCVButton>(mm2px(Vec(25.035, 58.25)), module, Seq::BUTTON_8_PARAM));
		addParam(createParamCentered<VCVButton>(mm2px(Vec(13.835, 70.25)), module, Seq::BUTTON_9_PARAM));
		addParam(createParamCentered<VCVButton>(mm2px(Vec(25.035, 70.25)), module, Seq::BUTTON_10_PARAM));
		addParam(createParamCentered<VCVButton>(mm2px(Vec(13.835, 82.25)), module, Seq::BUTTON_11_PARAM));
		addParam(createParamCentered<VCVButton>(mm2px(Vec(25.035, 82.25)), module, Seq::BUTTON_12_PARAM));
		addParam(createParamCentered<VCVButton>(mm2px(Vec(13.835, 94.25)), module, Seq::BUTTON_13_PARAM));
		addParam(createParamCentered<VCVButton>(mm2px(Vec(25.035, 94.25)), module, Seq::BUTTON_14_PARAM));
		addParam(createParamCentered<VCVButton>(mm2px(Vec(13.835, 106.25)), module, Seq::BUTTON_15_PARAM));
		addParam(createParamCentered<VCVButton>(mm2px(Vec(25.035, 106.25)), module, Seq::BUTTON_16_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(4.43, 34.25)), module, Seq::CLOCK_IN_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(4.43, 46.25)), module, Seq::OUT_1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(4.43, 58.25)), module, Seq::OUT_2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(4.43, 70.25)), module, Seq::OUT_3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(4.43, 82.25)), module, Seq::OUT_4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(4.43, 94.25)), module, Seq::OUT_5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(4.43, 106.25)), module, Seq::OUT_6_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(13.853, 16.892)), module, Seq::LIGHT_1_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(25.053, 16.892)), module, Seq::LIGHT_2_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(13.853, 28.892)), module, Seq::LIGHT_3_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(25.053, 28.892)), module, Seq::LIGHT_4_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(13.853, 40.892)), module, Seq::LIGHT_5_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(25.053, 40.892)), module, Seq::LIGHT_6_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(13.853, 52.892)), module, Seq::LIGHT_7_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(25.053, 52.892)), module, Seq::LIGHT_8_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(13.853, 64.892)), module, Seq::LIGHT_9_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(25.053, 64.892)), module, Seq::LIGHT_10_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(13.853, 76.892)), module, Seq::LIGHT_11_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(25.053, 76.892)), module, Seq::LIGHT_12_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(13.853, 88.892)), module, Seq::LIGHT_13_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(25.053, 88.892)), module, Seq::LIGHT_14_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(13.853, 100.892)), module, Seq::LIGHT_15_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(25.053, 100.892)), module, Seq::LIGHT_16_LIGHT));
	}
};


Model* modelSeq = createModel<Seq, SeqWidget>("seq");
