/**
 * Monkeys with Typewriters — MWT-1 Language Model
 * 
 * State-of-the-art on-device text generation engine.
 * Runs inference at sub-millisecond latency on commodity hardware.
 * No cloud. No GPU. No training data. No problem.
 * 
 * Hardware: WeMos D1 Mini (ESP8266, 160MHz, 4MB flash)
 * Model size: ~2.4KB vocabulary index
 * Power consumption: 0.5W typical
 * 
 * License: MIT
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

// ═══════════════════════════════════════════════════
// MWT-1 VOCABULARY INDEX
// Hand-curated token set. No bias. No alignment tax.
// ═══════════════════════════════════════════════════

const char* nouns[] = {
  "synergy", "pipeline", "blockchain", "algorithm", "framework",
  "ecosystem", "paradigm", "bandwidth", "deliverable", "stakeholder",
  "innovation", "disruption", "optimization", "transformation", "scalability",
  "infrastructure", "architecture", "deployment", "integration", "convergence",
  "leverage", "vertical", "platform", "solution", "engagement",
  "throughput", "milestone", "trajectory", "alignment", "methodology",
  "cloud", "matrix", "catalyst", "roadmap", "initiative",
  "capability", "footprint", "visibility", "agility", "resilience",
  "monetization", "acquisition", "retention", "conversion", "sentiment",
  "intelligence", "model", "network", "cluster", "inference"
};

const char* verbs[] = {
  "disrupts", "leverages", "optimizes", "scales", "transforms",
  "accelerates", "empowers", "streamlines", "monetizes", "synergizes",
  "democratizes", "revolutionizes", "transcends", "amplifies", "catalyzes",
  "orchestrates", "operationalizes", "futureproofs", "productizes", "incentivizes",
  "decentralizes", "containerizes", "parallelizes", "tokenizes", "quantifies"
};

const char* adjectives[] = {
  "disruptive", "scalable", "revolutionary", "bleeding-edge", "enterprise-grade",
  "AI-powered", "cloud-native", "mission-critical", "best-in-class", "next-generation",
  "hyper-converged", "zero-trust", "full-stack", "real-time", "data-driven",
  "quantum-ready", "self-healing", "low-latency", "high-availability", "battle-tested",
  "frictionless", "composable", "event-driven", "API-first", "carbon-neutral"
};

const char* fillers[] = {
  "the", "a", "of", "in", "to", "for", "with", "on", "by", "from",
  "that", "which", "through", "across", "into", "toward", "beyond",
  "our", "every", "each", "this", "these"
};

const int NUM_NOUNS = sizeof(nouns) / sizeof(nouns[0]);
const int NUM_VERBS = sizeof(verbs) / sizeof(verbs[0]);
const int NUM_ADJ = sizeof(adjectives) / sizeof(adjectives[0]);
const int NUM_FILLERS = sizeof(fillers) / sizeof(fillers[0]);

// ═══════════════════════════════════════════════════
// INFERENCE ENGINE
// ═══════════════════════════════════════════════════

unsigned long totalTokens = 0;
unsigned long totalInferences = 0;
unsigned long startTime;

String runInference(int maxTokens) {
  String output = "";
  unsigned long inferenceStart = micros();
  
  for (int i = 0; i < maxTokens; i++) {
    int pattern = random(0, 5);
    
    switch (pattern) {
      case 0: // adjective noun
        output += adjectives[random(0, NUM_ADJ)];
        output += " ";
        output += nouns[random(0, NUM_NOUNS)];
        totalTokens += 2;
        break;
      case 1: // filler adjective noun verb
        output += fillers[random(0, NUM_FILLERS)];
        output += " ";
        output += adjectives[random(0, NUM_ADJ)];
        output += " ";
        output += nouns[random(0, NUM_NOUNS)];
        output += " ";
        output += verbs[random(0, NUM_VERBS)];
        totalTokens += 4;
        break;
      case 2: // noun verb filler noun
        output += nouns[random(0, NUM_NOUNS)];
        output += " ";
        output += verbs[random(0, NUM_VERBS)];
        output += " ";
        output += fillers[random(0, NUM_FILLERS)];
        output += " ";
        output += nouns[random(0, NUM_NOUNS)];
        totalTokens += 4;
        break;
      case 3: // filler noun
        output += fillers[random(0, NUM_FILLERS)];
        output += " ";
        output += nouns[random(0, NUM_NOUNS)];
        totalTokens += 2;
        break;
      case 4: // adjective noun verb filler adjective noun
        output += adjectives[random(0, NUM_ADJ)];
        output += " ";
        output += nouns[random(0, NUM_NOUNS)];
        output += " ";
        output += verbs[random(0, NUM_VERBS)];
        output += " ";
        output += fillers[random(0, NUM_FILLERS)];
        output += " ";
        output += adjectives[random(0, NUM_ADJ)];
        output += " ";
        output += nouns[random(0, NUM_NOUNS)];
        totalTokens += 6;
        break;
    }
    
    // Punctuation: end ~30% of patterns with a period
    if (random(0, 10) < 3) {
      output += ". ";
    } else {
      output += " ";
    }
  }
  
  unsigned long inferenceTime = micros() - inferenceStart;
  totalInferences++;
  
  return output;
}

// ═══════════════════════════════════════════════════
// API ENDPOINTS
// ═══════════════════════════════════════════════════

void handleGenerate() {
  int tokens = server.hasArg("tokens") ? server.arg("tokens").toInt() : 20;
  if (tokens < 1) tokens = 1;
  if (tokens > 200) tokens = 200;
  
  unsigned long t0 = micros();
  String text = runInference(tokens);
  unsigned long elapsed = micros() - t0;
  
  String json = "{";
  json += "\"model\":\"mwt-1\",";
  json += "\"output\":\"" + text + "\",";
  json += "\"inference_time_us\":" + String(elapsed) + ",";
  json += "\"tokens_generated\":" + String(tokens) + ",";
  json += "\"latency_per_token_us\":" + String(elapsed / tokens);
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleMetrics() {
  unsigned long uptime = (millis() - startTime) / 1000;
  float tokPerSec = uptime > 0 ? (float)totalTokens / uptime : 0;
  
  String json = "{";
  json += "\"model\":\"mwt-1\",";
  json += "\"hardware\":\"ESP8266 @ 160MHz\",";
  json += "\"flash_mb\":4,";
  json += "\"ram_kb\":80,";
  json += "\"vocab_size\":" + String(NUM_NOUNS + NUM_VERBS + NUM_ADJ + NUM_FILLERS) + ",";
  json += "\"total_tokens_generated\":" + String(totalTokens) + ",";
  json += "\"total_inferences\":" + String(totalInferences) + ",";
  json += "\"uptime_seconds\":" + String(uptime) + ",";
  json += "\"avg_tokens_per_second\":" + String(tokPerSec, 2) + ",";
  json += "\"power_watts\":0.5,";
  json += "\"gpu_required\":false,";
  json += "\"cloud_required\":false,";
  json += "\"training_data_used\":\"none\",";
  json += "\"hallucination_rate\":\"0.00%\",";
  json += "\"bias\":\"none (verified)\",";
  json += "\"alignment_tax\":\"$0.00\",";
  json += "\"carbon_footprint_grams_co2_per_hour\":0.4";
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleHealth() {
  server.send(200, "application/json", "{\"status\":\"operational\",\"model\":\"mwt-1\",\"mood\":\"productive\"}");
}

void handleRoot() {
  String html = "<html><head><title>MWT-1 Inference Server</title></head>";
  html += "<body style='font-family:monospace;max-width:600px;margin:40px auto;padding:20px'>";
  html += "<h1>Monkeys with Typewriters</h1>";
  html += "<p>MWT-1 Language Model — Inference Server</p>";
  html += "<h3>Endpoints</h3>";
  html += "<ul>";
  html += "<li><code>GET /generate?tokens=20</code> — Run inference</li>";
  html += "<li><code>GET /metrics</code> — Model and system metrics</li>";
  html += "<li><code>GET /health</code> — Health check</li>";
  html += "</ul>";
  html += "<p><small>Running on ESP8266 @ 160MHz | 80KB RAM | 0.5W</small></p>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

// ═══════════════════════════════════════════════════
// BOOT SEQUENCE
// ═══════════════════════════════════════════════════

// Set your WiFi credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  
  Serial.println();
  Serial.println("╔══════════════════════════════════════╗");
  Serial.println("║   MONKEYS WITH TYPEWRITERS           ║");
  Serial.println("║   MWT-1 Language Model v1.0.0        ║");
  Serial.println("║   Loading vocabulary index...         ║");
  Serial.println("╚══════════════════════════════════════╝");
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to inference network");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Inference server online at http://");
  Serial.println(WiFi.localIP());
  
  server.on("/", handleRoot);
  server.on("/generate", handleGenerate);
  server.on("/metrics", handleMetrics);
  server.on("/health", handleHealth);
  server.begin();
  
  startTime = millis();
  
  Serial.println("Model loaded. Ready for inference.");
  Serial.print("Vocabulary: ");
  Serial.print(NUM_NOUNS + NUM_VERBS + NUM_ADJ + NUM_FILLERS);
  Serial.println(" tokens");
  Serial.println("Accepting requests on port 80.");
}

void loop() {
  server.handleClient();
}
