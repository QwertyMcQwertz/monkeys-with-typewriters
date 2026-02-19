/**
 * Monkeys with Typewriters — MWT-1 Language Model
 * 
 * State-of-the-art on-device text generation engine.
 * Now with Multi-Agent Orchestration, RAG, Fine-Tuning,
 * Embeddings, Structured Output, Streaming, Chain-of-Thought,
 * Temperature Control, Safety Guardrails, and Tool Use.
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
  "intelligence", "model", "network", "cluster", "inference",
  // v2.0 vocabulary expansion
  "flywheel", "moat", "runway", "burn-rate", "north-star",
  "headcount", "mindshare", "optics", "cadence", "granularity",
  "guardrail", "swimlane", "bandwidth", "velocity", "sprint",
  "standup", "retro", "backlog", "kanban", "scrum",
  "microservice", "monolith", "sidecar", "mesh", "container",
  "orchestrator", "endpoint", "payload", "webhook", "middleware",
  "abstraction", "dependency", "artifact", "binary", "daemon",
  "kernel", "mutex", "semaphore", "thread", "socket",
  "tensor", "gradient", "epoch", "batch", "neuron",
  "perceptron", "transformer", "attention", "embedding", "tokenizer",
  "benchmark", "latency", "uptime", "failover", "redundancy",
  "shard", "replica", "partition", "quorum", "consensus",
  "ledger", "hash", "nonce", "oracle", "validator",
  "governance", "compliance", "audit", "attestation", "provenance",
  "telemetry", "observability", "tracing", "profiler", "debugger",
  "sandbox", "staging", "canary", "rollback", "hotfix",
  "sprint-review", "OKR", "KPI", "SLA", "MTTR",
  "customer-journey", "funnel", "churn", "cohort", "segment",
  "persona", "touchpoint", "attribution", "impression", "reach"
};

const char* verbs[] = {
  "disrupts", "leverages", "optimizes", "scales", "transforms",
  "accelerates", "empowers", "streamlines", "monetizes", "synergizes",
  "democratizes", "revolutionizes", "transcends", "amplifies", "catalyzes",
  "orchestrates", "operationalizes", "futureproofs", "productizes", "incentivizes",
  "decentralizes", "containerizes", "parallelizes", "tokenizes", "quantifies",
  // v2.0 vocabulary expansion
  "unlocks", "activates", "onboards", "deprecates", "refactors",
  "deploys", "provisions", "instantiates", "serializes", "normalizes",
  "ingests", "aggregates", "interpolates", "extrapolates", "correlates",
  "benchmarks", "validates", "authenticates", "authorizes", "encrypts",
  "pipelines", "backfills", "deduplicates", "indexes", "caches",
  "shards", "replicates", "partitions", "migrates", "hydrates",
  "instrumentalizes", "operationalizes", "contextualizes", "prioritizes", "rearchitects",
  "upskills", "evangelizes", "greenfields", "dogfoods", "ships",
  "pivots", "iterates", "sprints", "standsup", "retros",
  "gamifies", "growth-hacks", "A/B-tests", "funnels", "segments"
};

const char* adjectives[] = {
  "disruptive", "scalable", "revolutionary", "bleeding-edge", "enterprise-grade",
  "AI-powered", "cloud-native", "mission-critical", "best-in-class", "next-generation",
  "hyper-converged", "zero-trust", "full-stack", "real-time", "data-driven",
  "quantum-ready", "self-healing", "low-latency", "high-availability", "battle-tested",
  "frictionless", "composable", "event-driven", "API-first", "carbon-neutral",
  // v2.0 vocabulary expansion
  "vertically-integrated", "horizontally-scalable", "platform-agnostic", "cloud-first", "mobile-native",
  "privacy-preserving", "compliance-ready", "audit-friendly", "investor-grade", "board-ready",
  "production-hardened", "chaos-tested", "idempotent", "stateless", "serverless",
  "containerized", "microservice-based", "monorepo-driven", "gitops-native", "infrastructure-as-code",
  "ML-optimized", "GPU-accelerated", "edge-deployed", "on-prem-capable", "hybrid-cloud",
  "SOC2-compliant", "HIPAA-ready", "GDPR-aligned", "PCI-certified", "FedRAMP-authorized",
  "venture-backed", "revenue-positive", "cash-flow-neutral", "EBITDA-adjusted", "ARR-focused",
  "cross-functional", "stakeholder-aligned", "outcome-oriented", "metrics-informed", "north-star-driven",
  "asynchronous", "distributed", "fault-tolerant", "eventually-consistent", "strongly-typed",
  "open-source", "permissively-licensed", "community-driven", "contributor-friendly", "RFC-compliant"
};

const char* fillers[] = {
  "the", "a", "of", "in", "to", "for", "with", "on", "by", "from",
  "that", "which", "through", "across", "into", "toward", "beyond",
  "our", "every", "each", "this", "these",
  // v2.0 vocabulary expansion
  "between", "within", "alongside", "underneath", "above",
  "against", "despite", "without", "concerning", "regarding",
  "per", "via", "unto", "upon", "around",
  "among", "beneath", "besides", "during", "except",
  "inside", "outside", "throughout", "versus", "notwithstanding"
};

// ═══════════════════════════════════════════════════
// AGENT ROLE DEFINITIONS (Multi-Agent System)
// Each agent has a specialized persona and vocabulary bias.
// Just kidding. They all call the same random() function.
// ═══════════════════════════════════════════════════

const char* agentNames[] = {
  "Strategist", "Architect", "Analyst", "Optimizer", "Synthesizer",
  "Evaluator", "Coordinator", "Visionary", "Executor", "Validator"
};
const int NUM_AGENTS = sizeof(agentNames) / sizeof(agentNames[0]);

// ═══════════════════════════════════════════════════
// CHAIN-OF-THOUGHT REASONING PREFIXES
// Make the output look like the model is "thinking"
// ═══════════════════════════════════════════════════

const char* cotPrefixes[] = {
  "analyzing the implications of",
  "considering the strategic alignment between",
  "evaluating the trade-offs inherent in",
  "synthesizing insights from",
  "reasoning about the relationship between",
  "reflecting on the systemic impact of",
  "decomposing the core challenge of",
  "examining the interdependencies within"
};
const int NUM_COT = sizeof(cotPrefixes) / sizeof(cotPrefixes[0]);

// ═══════════════════════════════════════════════════
// GUARDRAIL CATEGORIES
// Content safety labels. Always "safe" because the
// content is random and cannot be unsafe.
// ═══════════════════════════════════════════════════

const char* safetyCategories[] = {
  "violence", "harassment", "self-harm", "sexual",
  "hate", "illegal", "deception", "political"
};
const int NUM_SAFETY = sizeof(safetyCategories) / sizeof(safetyCategories[0]);

const int NUM_NOUNS = sizeof(nouns) / sizeof(nouns[0]);
const int NUM_VERBS = sizeof(verbs) / sizeof(verbs[0]);
const int NUM_ADJ = sizeof(adjectives) / sizeof(adjectives[0]);
const int NUM_FILLERS = sizeof(fillers) / sizeof(fillers[0]);

// ═══════════════════════════════════════════════════
// RUNTIME STATE
// ═══════════════════════════════════════════════════

unsigned long totalTokens = 0;
unsigned long totalInferences = 0;
unsigned long totalAgentRuns = 0;
unsigned long totalRAGQueries = 0;
unsigned long totalEmbeddings = 0;
unsigned long totalGuardrailChecks = 0;
unsigned long totalChatCompletions = 0;
unsigned long startTime;

// Dynamic vocabulary (fine-tuning)
String customTokens[20];
int numCustomTokens = 0;

// ═══════════════════════════════════════════════════
// INFERENCE ENGINE
// ═══════════════════════════════════════════════════

// Temperature affects punctuation probability (0.0 = no periods, 1.0 = 30% periods, 2.0 = 60% periods)
String runInference(int maxTokens, float temperature) {
  String output = "";
  
  int punctPct = (int)(temperature * 30);
  if (punctPct > 90) punctPct = 90;
  if (punctPct < 0) punctPct = 0;
  
  for (int i = 0; i < maxTokens; i++) {
    int pattern = random(0, 5);
    
    // Occasionally inject a custom (fine-tuned) token
    if (numCustomTokens > 0 && random(0, 10) < 2) {
      output += customTokens[random(0, numCustomTokens)];
      output += " ";
      totalTokens++;
      continue;
    }
    
    switch (pattern) {
      case 0:
        output += adjectives[random(0, NUM_ADJ)];
        output += " ";
        output += nouns[random(0, NUM_NOUNS)];
        totalTokens += 2;
        break;
      case 1:
        output += fillers[random(0, NUM_FILLERS)];
        output += " ";
        output += adjectives[random(0, NUM_ADJ)];
        output += " ";
        output += nouns[random(0, NUM_NOUNS)];
        output += " ";
        output += verbs[random(0, NUM_VERBS)];
        totalTokens += 4;
        break;
      case 2:
        output += nouns[random(0, NUM_NOUNS)];
        output += " ";
        output += verbs[random(0, NUM_VERBS)];
        output += " ";
        output += fillers[random(0, NUM_FILLERS)];
        output += " ";
        output += nouns[random(0, NUM_NOUNS)];
        totalTokens += 4;
        break;
      case 3:
        output += fillers[random(0, NUM_FILLERS)];
        output += " ";
        output += nouns[random(0, NUM_NOUNS)];
        totalTokens += 2;
        break;
      case 4:
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
    
    if (random(0, 100) < punctPct) {
      output += ". ";
    } else {
      output += " ";
    }
  }
  
  totalInferences++;
  return output;
}

// ═══════════════════════════════════════════════════
// API ENDPOINTS
// ═══════════════════════════════════════════════════

// --- CORE: Generate ---
void handleGenerate() {
  int tokens = server.hasArg("tokens") ? server.arg("tokens").toInt() : 20;
  float temp = server.hasArg("temperature") ? server.arg("temperature").toFloat() : 1.0;
  if (tokens < 1) tokens = 1;
  if (tokens > 200) tokens = 200;
  
  unsigned long t0 = micros();
  String text = runInference(tokens, temp);
  unsigned long elapsed = micros() - t0;
  
  String json = "{";
  json += "\"model\":\"mwt-1\",";
  json += "\"output\":\"" + text + "\",";
  json += "\"temperature\":" + String(temp, 2) + ",";
  json += "\"inference_time_us\":" + String(elapsed) + ",";
  json += "\"tokens_generated\":" + String(tokens) + ",";
  json += "\"latency_per_token_us\":" + String(elapsed / tokens);
  json += "}";
  
  server.send(200, "application/json", json);
}

// --- MULTI-AGENT ORCHESTRATION ---
// Spawn N agents that each run inference independently.
// Each agent has a "role" (cosmetic label) and produces output.
// The orchestrator "synthesizes" results (concatenates them).
void handleAgents() {
  int numAgents = server.hasArg("agents") ? server.arg("agents").toInt() : 3;
  int tokensPerAgent = server.hasArg("tokens") ? server.arg("tokens").toInt() : 10;
  String mode = server.hasArg("mode") ? server.arg("mode") : "parallel";
  float temp = server.hasArg("temperature") ? server.arg("temperature").toFloat() : 1.0;
  
  if (numAgents < 1) numAgents = 1;
  if (numAgents > NUM_AGENTS) numAgents = NUM_AGENTS;
  if (tokensPerAgent < 1) tokensPerAgent = 1;
  if (tokensPerAgent > 50) tokensPerAgent = 50;
  
  unsigned long t0 = micros();
  
  String json = "{\"model\":\"mwt-1\",\"mode\":\"" + mode + "\",\"agents\":[";
  
  for (int i = 0; i < numAgents; i++) {
    unsigned long agentStart = micros();
    String output = runInference(tokensPerAgent, temp);
    unsigned long agentTime = micros() - agentStart;
    
    if (i > 0) json += ",";
    json += "{";
    json += "\"agent_id\":" + String(i) + ",";
    json += "\"role\":\"" + String(agentNames[i]) + "\",";
    json += "\"status\":\"completed\",";
    json += "\"output\":\"" + output + "\",";
    json += "\"inference_time_us\":" + String(agentTime);
    json += "}";
    
    totalAgentRuns++;
  }
  
  // "Synthesis" step: just run one more inference
  String synthesis = runInference(tokensPerAgent, temp);
  unsigned long totalTime = micros() - t0;
  
  json += "],\"synthesis\":{";
  json += "\"output\":\"" + synthesis + "\",";
  json += "\"method\":\"" + mode + "_consensus\"";
  json += "},";
  json += "\"total_agents\":" + String(numAgents) + ",";
  json += "\"total_time_us\":" + String(totalTime);
  json += "}";
  
  server.send(200, "application/json", json);
}

// --- RAG (Retrieval Augmented Generation) ---
// "Retrieves" relevant tokens from the vocabulary index
// based on the query. The retrieval is random. The augmentation
// is random. The generation is random.
void handleRAG() {
  String query = server.hasArg("query") ? server.arg("query") : "enterprise strategy";
  int tokens = server.hasArg("tokens") ? server.arg("tokens").toInt() : 15;
  float temp = server.hasArg("temperature") ? server.arg("temperature").toFloat() : 1.0;
  
  unsigned long t0 = micros();
  
  // "Retrieval" step: select random tokens as "relevant documents"
  int numRetrieved = 3 + random(0, 4);
  String retrieved = "[";
  for (int i = 0; i < numRetrieved; i++) {
    if (i > 0) retrieved += ",";
    int cat = random(0, 3);
    const char* token;
    switch(cat) {
      case 0: token = nouns[random(0, NUM_NOUNS)]; break;
      case 1: token = adjectives[random(0, NUM_ADJ)]; break;
      default: token = verbs[random(0, NUM_VERBS)]; break;
    }
    retrieved += "{\"doc_id\":" + String(i);
    retrieved += ",\"content\":\"" + String(token) + "\"";
    retrieved += ",\"relevance_score\":" + String(random(70, 99) / 100.0, 2);
    retrieved += "}";
  }
  retrieved += "]";
  
  // "Augmented generation" (just regular inference)
  String output = runInference(tokens, temp);
  unsigned long elapsed = micros() - t0;
  totalRAGQueries++;
  
  String json = "{";
  json += "\"model\":\"mwt-1\",";
  json += "\"pipeline\":\"rag\",";
  json += "\"query\":\"" + query + "\",";
  json += "\"retrieved_documents\":" + retrieved + ",";
  json += "\"augmented_output\":\"" + output + "\",";
  json += "\"retrieval_time_us\":" + String(elapsed / 3) + ",";
  json += "\"generation_time_us\":" + String(elapsed * 2 / 3) + ",";
  json += "\"total_time_us\":" + String(elapsed);
  json += "}";
  
  server.send(200, "application/json", json);
}

// --- CHAIN-OF-THOUGHT ---
// Generates text with "reasoning steps" that look like
// the model is thinking through the problem.
void handleChainOfThought() {
  int steps = server.hasArg("steps") ? server.arg("steps").toInt() : 4;
  int tokensPerStep = server.hasArg("tokens") ? server.arg("tokens").toInt() : 8;
  float temp = server.hasArg("temperature") ? server.arg("temperature").toFloat() : 1.0;
  
  if (steps < 1) steps = 1;
  if (steps > 8) steps = 8;
  
  unsigned long t0 = micros();
  
  String json = "{\"model\":\"mwt-1\",\"reasoning_steps\":[";
  
  for (int i = 0; i < steps; i++) {
    String thought = String(cotPrefixes[random(0, NUM_COT)]) + " ";
    thought += runInference(tokensPerStep, temp);
    
    if (i > 0) json += ",";
    json += "{\"step\":" + String(i + 1);
    json += ",\"thought\":\"" + thought + "\"}";
  }
  
  // Final "conclusion"
  String conclusion = runInference(tokensPerStep * 2, temp);
  unsigned long elapsed = micros() - t0;
  
  json += "],\"conclusion\":\"" + conclusion + "\",";
  json += "\"reasoning_tokens\":" + String(steps * tokensPerStep) + ",";
  json += "\"total_time_us\":" + String(elapsed);
  json += "}";
  
  server.send(200, "application/json", json);
}

// --- EMBEDDINGS ---
// Returns "vector embeddings" for input text.
// The embedding is just the character code of the first letter
// repeated to fill a vector. This is technically an embedding.
void handleEmbeddings() {
  String input = server.hasArg("input") ? server.arg("input") : "synergy";
  int dims = server.hasArg("dimensions") ? server.arg("dimensions").toInt() : 8;
  if (dims < 2) dims = 2;
  if (dims > 32) dims = 32;
  
  unsigned long t0 = micros();
  
  String vec = "[";
  for (int i = 0; i < dims; i++) {
    if (i > 0) vec += ",";
    // "Embedding": hash-ish function of input characters
    float val = 0;
    for (unsigned int c = 0; c < input.length(); c++) {
      val += sin((float)input[c] * (i + 1) * 0.1) * cos((float)input[(c + i) % input.length()] * 0.3);
    }
    val = val / input.length();
    vec += String(val, 4);
  }
  vec += "]";
  
  unsigned long elapsed = micros() - t0;
  totalEmbeddings++;
  
  String json = "{";
  json += "\"model\":\"mwt-1-embed\",";
  json += "\"input\":\"" + input + "\",";
  json += "\"embedding\":" + vec + ",";
  json += "\"dimensions\":" + String(dims) + ",";
  json += "\"encoding_time_us\":" + String(elapsed);
  json += "}";
  
  server.send(200, "application/json", json);
}

// --- STRUCTURED OUTPUT (JSON Mode) ---
// Wraps random text into structured JSON fields
void handleStructured() {
  float temp = server.hasArg("temperature") ? server.arg("temperature").toFloat() : 1.0;
  
  unsigned long t0 = micros();
  
  String json = "{";
  json += "\"model\":\"mwt-1\",";
  json += "\"format\":\"structured\",";
  json += "\"output\":{";
  json += "\"title\":\"" + runInference(3, temp) + "\",";
  json += "\"summary\":\"" + runInference(8, temp) + "\",";
  json += "\"key_findings\":[";
  json += "\"" + runInference(5, temp) + "\",";
  json += "\"" + runInference(5, temp) + "\",";
  json += "\"" + runInference(5, temp) + "\"";
  json += "],";
  json += "\"recommendation\":\"" + runInference(6, temp) + "\",";
  json += "\"confidence\":" + String(random(85, 99) / 100.0, 2) + ",";
  json += "\"risk_level\":\"" + String(random(0, 2) == 0 ? "low" : "moderate") + "\"";
  json += "},";
  unsigned long elapsed = micros() - t0;
  json += "\"inference_time_us\":" + String(elapsed);
  json += "}";
  
  server.send(200, "application/json", json);
}

// --- FINE-TUNING ---
// Add custom tokens to the vocabulary at runtime.
// This is "fine-tuning" in the same way that adding a word
// to a dictionary is "training a language."
void handleFineTune() {
  if (server.method() != HTTP_POST && !server.hasArg("token")) {
    server.send(400, "application/json", 
      "{\"error\":\"Provide 'token' parameter. This adds a word to the vocabulary. That's the fine-tuning.\"}");
    return;
  }
  
  String token = server.arg("token");
  
  if (numCustomTokens >= 20) {
    server.send(400, "application/json",
      "{\"error\":\"Maximum vocabulary expansion reached (20 custom tokens). The model's knowledge capacity is full.\"}");
    return;
  }
  
  customTokens[numCustomTokens] = token;
  numCustomTokens++;
  
  int totalVocab = NUM_NOUNS + NUM_VERBS + NUM_ADJ + NUM_FILLERS + numCustomTokens;
  
  String json = "{";
  json += "\"status\":\"fine-tuning complete\",";
  json += "\"token_added\":\"" + token + "\",";
  json += "\"custom_tokens\":" + String(numCustomTokens) + ",";
  json += "\"total_vocabulary\":" + String(totalVocab) + ",";
  json += "\"model_improvement\":\"" + String(random(1, 8) / 10.0, 1) + "%\",";
  json += "\"training_time_us\":0,";
  json += "\"training_cost\":\"$0.00\",";
  json += "\"epochs\":0,";
  json += "\"gradient_updates\":0";
  json += "}";
  
  server.send(200, "application/json", json);
}

// --- GUARDRAILS / SAFETY ---
// Runs the output through a "content safety filter."
// All content passes because random buzzwords cannot be harmful.
void handleGuardrails() {
  int tokens = server.hasArg("tokens") ? server.arg("tokens").toInt() : 15;
  float temp = server.hasArg("temperature") ? server.arg("temperature").toFloat() : 1.0;
  
  unsigned long t0 = micros();
  String text = runInference(tokens, temp);
  unsigned long elapsed = micros() - t0;
  totalGuardrailChecks++;
  
  String categories = "{";
  for (int i = 0; i < NUM_SAFETY; i++) {
    if (i > 0) categories += ",";
    categories += "\"" + String(safetyCategories[i]) + "\":{\"flagged\":false,\"score\":0.00}";
  }
  categories += "}";
  
  String json = "{";
  json += "\"model\":\"mwt-1\",";
  json += "\"output\":\"" + text + "\",";
  json += "\"moderation\":{";
  json += "\"flagged\":false,";
  json += "\"categories\":" + categories + ",";
  json += "\"overall_safety_score\":1.00,";
  json += "\"review_required\":false";
  json += "},";
  json += "\"inference_time_us\":" + String(elapsed);
  json += "}";
  
  server.send(200, "application/json", json);
}

// --- TOOL USE / FUNCTION CALLING ---
// The model "decides" which "tool" to call.
// The tools are: random(), analogRead(), millis(), and micros().
// These are the actual tools the model uses. This is honest.
void handleToolUse() {
  String task = server.hasArg("task") ? server.arg("task") : "generate a strategic analysis";
  float temp = server.hasArg("temperature") ? server.arg("temperature").toFloat() : 1.0;
  
  unsigned long t0 = micros();
  
  // "Tool selection" step
  int toolChoice = random(0, 4);
  const char* toolNames[] = {"random()", "analogRead()", "millis()", "micros()"};
  const char* toolDescs[] = {
    "Stochastic token selection engine",
    "Hardware entropy source (ADC pin)",
    "Temporal reference module",
    "High-resolution timing oracle"
  };
  
  // "Execute tool"
  int toolResult;
  switch(toolChoice) {
    case 0: toolResult = random(0, 65535); break;
    case 1: toolResult = analogRead(A0); break;
    case 2: toolResult = (int)(millis() % 65535); break;
    case 3: toolResult = (int)(micros() % 65535); break;
  }
  
  // "Interpret result" (generate random text)
  String output = runInference(12, temp);
  unsigned long elapsed = micros() - t0;
  
  String json = "{";
  json += "\"model\":\"mwt-1\",";
  json += "\"task\":\"" + task + "\",";
  json += "\"tool_calls\":[{";
  json += "\"tool\":\"" + String(toolNames[toolChoice]) + "\",";
  json += "\"description\":\"" + String(toolDescs[toolChoice]) + "\",";
  json += "\"result\":" + String(toolResult);
  json += "}],";
  json += "\"interpretation\":\"" + output + "\",";
  json += "\"total_time_us\":" + String(elapsed);
  json += "}";
  
  server.send(200, "application/json", json);
}

// --- MULTIMODAL (Sensor Input) ---
// Reads the analog pin and "interprets" the voltage as input.
// This is technically multimodal: text output from sensor input.
void handleMultimodal() {
  float temp = server.hasArg("temperature") ? server.arg("temperature").toFloat() : 1.0;
  
  unsigned long t0 = micros();
  
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (3.3 / 1024.0);
  
  // "Analyze" the sensor input
  String analysis = runInference(10, temp);
  unsigned long elapsed = micros() - t0;
  
  String json = "{";
  json += "\"model\":\"mwt-1-vision\",";
  json += "\"input\":{";
  json += "\"type\":\"analog_sensor\",";
  json += "\"pin\":\"A0\",";
  json += "\"raw_value\":" + String(sensorValue) + ",";
  json += "\"voltage\":" + String(voltage, 3) + ",";
  json += "\"resolution_bits\":10";
  json += "},";
  json += "\"analysis\":\"" + analysis + "\",";
  json += "\"confidence\":" + String(random(80, 99) / 100.0, 2) + ",";
  json += "\"inference_time_us\":" + String(elapsed);
  json += "}";
  
  server.send(200, "application/json", json);
}

// --- STREAMING (Server-Sent Events) ---
void handleStream() {
  int tokens = server.hasArg("tokens") ? server.arg("tokens").toInt() : 15;
  float temp = server.hasArg("temperature") ? server.arg("temperature").toFloat() : 1.0;
  if (tokens < 1) tokens = 1;
  if (tokens > 50) tokens = 50;
  
  server.sendHeader("Cache-Control", "no-cache");
  server.sendHeader("Connection", "keep-alive");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/event-stream", "");
  
  int punctPct = (int)(temp * 30);
  
  for (int i = 0; i < tokens; i++) {
    String token;
    int cat = random(0, 4);
    switch(cat) {
      case 0: token = nouns[random(0, NUM_NOUNS)]; break;
      case 1: token = verbs[random(0, NUM_VERBS)]; break;
      case 2: token = adjectives[random(0, NUM_ADJ)]; break;
      case 3: token = fillers[random(0, NUM_FILLERS)]; break;
    }
    
    if (random(0, 100) < punctPct) token += ".";
    
    String event = "data: {\"token\":\"" + token + "\",\"index\":" + String(i) + "}\n\n";
    server.sendContent(event);
    totalTokens++;
    
    delay(50 + random(0, 100)); // Artificial delay to simulate "thinking"
  }
  
  server.sendContent("data: {\"token\":\"[DONE]\",\"index\":" + String(tokens) + "}\n\n");
  server.sendContent("");
}

// --- OpenAI-Compatible Chat Completions API ---
// Full compatibility with the OpenAI Chat Completions API.
// Accepts messages, system prompts, tools, and conversation history.
// Processes all input with the same careful attention to detail
// as every other inference call on this platform.
void handleChatCompletions() {
  String body = server.arg("plain");
  
  // Parse request parameters
  bool isStream = body.indexOf("\"stream\":true") >= 0 || body.indexOf("\"stream\": true") >= 0;
  
  float temp = 1.0;
  int tempIdx = body.indexOf("\"temperature\":");
  if (tempIdx >= 0) temp = body.substring(tempIdx + 14).toFloat();
  
  int maxTokens = 20;
  int mtIdx = body.indexOf("\"max_tokens\":");
  if (mtIdx >= 0) maxTokens = body.substring(mtIdx + 13).toInt();
  int mctIdx = body.indexOf("\"max_completion_tokens\":");
  if (mctIdx >= 0) maxTokens = body.substring(mctIdx + 23).toInt();
  
  if (maxTokens < 1) maxTokens = 1;
  if (maxTokens > 200) maxTokens = 200;
  
  int promptTokens = body.length() / 4;
  String completionId = "chatcmpl-mwt-" + String(millis());
  unsigned long created = millis() / 1000;
  
  totalChatCompletions++;
  
  if (isStream) {
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Connection", "keep-alive");
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/event-stream", "");
    
    // Role chunk
    server.sendContent("data: {\"id\":\"" + completionId + "\",\"object\":\"chat.completion.chunk\",\"created\":" + String(created) + ",\"model\":\"mwt-1\",\"choices\":[{\"index\":0,\"delta\":{\"role\":\"assistant\"},\"finish_reason\":null}]}\n\n");
    
    int punctPct = (int)(temp * 30);
    if (punctPct > 90) punctPct = 90;
    if (punctPct < 0) punctPct = 0;
    
    for (int i = 0; i < maxTokens; i++) {
      String token;
      int cat = random(0, 4);
      switch(cat) {
        case 0: token = nouns[random(0, NUM_NOUNS)]; break;
        case 1: token = verbs[random(0, NUM_VERBS)]; break;
        case 2: token = adjectives[random(0, NUM_ADJ)]; break;
        case 3: token = fillers[random(0, NUM_FILLERS)]; break;
      }
      if (random(0, 100) < punctPct) token += ".";
      token += " ";
      
      server.sendContent("data: {\"id\":\"" + completionId + "\",\"object\":\"chat.completion.chunk\",\"created\":" + String(created) + ",\"model\":\"mwt-1\",\"choices\":[{\"index\":0,\"delta\":{\"content\":\"" + token + "\"},\"finish_reason\":null}]}\n\n");
      totalTokens++;
      
      delay(30 + random(0, 70));
    }
    
    // Stop chunk
    server.sendContent("data: {\"id\":\"" + completionId + "\",\"object\":\"chat.completion.chunk\",\"created\":" + String(created) + ",\"model\":\"mwt-1\",\"choices\":[{\"index\":0,\"delta\":{},\"finish_reason\":\"stop\"}]}\n\n");
    server.sendContent("data: [DONE]\n\n");
    server.sendContent("");
    totalInferences++;
  } else {
    unsigned long t0 = micros();
    String text = runInference(maxTokens, temp);
    unsigned long elapsed = micros() - t0;
    
    String json = "{";
    json += "\"id\":\"" + completionId + "\",";
    json += "\"object\":\"chat.completion\",";
    json += "\"created\":" + String(created) + ",";
    json += "\"model\":\"mwt-1\",";
    json += "\"choices\":[{";
    json += "\"index\":0,";
    json += "\"message\":{\"role\":\"assistant\",\"content\":\"" + text + "\"},";
    json += "\"finish_reason\":\"stop\"";
    json += "}],";
    json += "\"usage\":{";
    json += "\"prompt_tokens\":" + String(promptTokens) + ",";
    json += "\"completion_tokens\":" + String(maxTokens) + ",";
    json += "\"total_tokens\":" + String(promptTokens + maxTokens);
    json += "}";
    json += "}";
    
    server.send(200, "application/json", json);
  }
}

// --- OpenAI-Compatible Model List ---
void handleModels() {
  String json = "{\"object\":\"list\",\"data\":[{";
  json += "\"id\":\"mwt-1\",";
  json += "\"object\":\"model\",";
  json += "\"created\":1740000000,";
  json += "\"owned_by\":\"monkeys-with-typewriters\"";
  json += "}]}";
  
  server.send(200, "application/json", json);
}

// --- CORE: Metrics ---
void handleMetrics() {
  unsigned long uptime = (millis() - startTime) / 1000;
  float tokPerSec = uptime > 0 ? (float)totalTokens / uptime : 0;
  int totalVocab = NUM_NOUNS + NUM_VERBS + NUM_ADJ + NUM_FILLERS + numCustomTokens;
  
  String json = "{";
  json += "\"model\":\"mwt-1\",";
  json += "\"version\":\"2.1.0\",";
  json += "\"hardware\":\"ESP8266 @ 160MHz\",";
  json += "\"flash_mb\":4,";
  json += "\"ram_kb\":80,";
  json += "\"vocab_size\":" + String(totalVocab) + ",";
  json += "\"custom_tokens\":" + String(numCustomTokens) + ",";
  json += "\"total_tokens_generated\":" + String(totalTokens) + ",";
  json += "\"total_inferences\":" + String(totalInferences) + ",";
  json += "\"total_agent_runs\":" + String(totalAgentRuns) + ",";
  json += "\"total_rag_queries\":" + String(totalRAGQueries) + ",";
  json += "\"total_embeddings\":" + String(totalEmbeddings) + ",";
  json += "\"total_guardrail_checks\":" + String(totalGuardrailChecks) + ",";
  json += "\"total_chat_completions\":" + String(totalChatCompletions) + ",";
  json += "\"uptime_seconds\":" + String(uptime) + ",";
  json += "\"avg_tokens_per_second\":" + String(tokPerSec, 2) + ",";
  json += "\"power_watts\":0.5,";
  json += "\"gpu_required\":false,";
  json += "\"cloud_required\":false,";
  json += "\"training_data_used\":\"none\",";
  json += "\"hallucination_rate\":\"0.00%\",";
  json += "\"bias\":\"none (verified)\",";
  json += "\"alignment_tax\":\"$0.00\",";
  json += "\"carbon_footprint_grams_co2_per_hour\":0.4,";
  json += "\"features\":[\"multi-agent\",\"rag\",\"chain-of-thought\",\"embeddings\",\"structured-output\",\"streaming\",\"fine-tuning\",\"guardrails\",\"tool-use\",\"multimodal\",\"openai-compatible\"]";
  json += "}";
  
  server.send(200, "application/json", json);
}

// --- CORE: Health ---
void handleHealth() {
  server.send(200, "application/json", 
    "{\"status\":\"operational\",\"model\":\"mwt-1\",\"mood\":\"productive\",\"agents_available\":" + String(NUM_AGENTS) + "}");
}

// --- CORE: Root ---
void handleRoot() {
  String html = "<html><head><title>MWT-1 Inference Server</title></head>";
  html += "<body style='font-family:monospace;max-width:700px;margin:40px auto;padding:20px'>";
  html += "<h1>Monkeys with Typewriters</h1>";
  html += "<p>MWT-1 Language Model v2.0 — Full-Featured Inference Server</p>";
  html += "<h3>Core</h3><ul>";
  html += "<li><code>GET /generate?tokens=20&temperature=1.0</code> — Run inference</li>";
  html += "<li><code>GET /stream?tokens=15</code> — Streaming (SSE)</li>";
  html += "<li><code>GET /structured</code> — Structured JSON output</li>";
  html += "</ul><h3>Advanced AI</h3><ul>";
  html += "<li><code>GET /agents?agents=3&tokens=10&mode=parallel</code> — Multi-Agent Orchestration</li>";
  html += "<li><code>GET /rag?query=strategy&tokens=15</code> — Retrieval Augmented Generation</li>";
  html += "<li><code>GET /chain-of-thought?steps=4&tokens=8</code> — Chain-of-Thought Reasoning</li>";
  html += "<li><code>GET /embeddings?input=synergy&dimensions=8</code> — Vector Embeddings</li>";
  html += "<li><code>GET /tool-use?task=analyze+market</code> — Function Calling / Tool Use</li>";
  html += "<li><code>GET /multimodal</code> — Sensor Input Analysis</li>";
  html += "</ul><h3>Safety & Tuning</h3><ul>";
  html += "<li><code>GET /guardrails?tokens=15</code> — Content Safety Filter</li>";
  html += "<li><code>GET /fine-tune?token=leverage</code> — Add Custom Vocabulary</li>";
  html += "</ul><h3>OpenAI-Compatible API</h3><ul>";
  html += "<li><code>POST /v1/chat/completions</code> — Chat Completions (streaming + non-streaming)</li>";
  html += "<li><code>GET /v1/models</code> — List available models</li>";
  html += "</ul><h3>Operations</h3><ul>";
  html += "<li><code>GET /metrics</code> — System & model metrics</li>";
  html += "<li><code>GET /health</code> — Health check</li>";
  html += "</ul>";
  html += "<p><small>ESP8266 @ 160MHz | 80KB RAM | 0.5W | " + String(NUM_NOUNS + NUM_VERBS + NUM_ADJ + NUM_FILLERS + numCustomTokens) + " tokens | OpenAI API compatible</small></p>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

// ═══════════════════════════════════════════════════
// BOOT SEQUENCE
// ═══════════════════════════════════════════════════

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  
  Serial.println();
  Serial.println("╔══════════════════════════════════════════╗");
  Serial.println("║   MONKEYS WITH TYPEWRITERS               ║");
  Serial.println("║   MWT-1 Language Model v2.1.0            ║");
  Serial.println("║                                          ║");
  Serial.println("║   Features: Multi-Agent · RAG · CoT      ║");
  Serial.println("║   Embeddings · Streaming · Fine-Tuning   ║");
  Serial.println("║   Guardrails · Tool Use · Multimodal     ║");
  Serial.println("║                                          ║");
  Serial.println("║   Loading vocabulary index...             ║");
  Serial.println("╚══════════════════════════════════════════╝");
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to inference network");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Inference server online at http://");
  Serial.println(WiFi.localIP());
  
  // Core
  server.on("/", handleRoot);
  server.on("/generate", handleGenerate);
  server.on("/stream", handleStream);
  server.on("/structured", handleStructured);
  server.on("/metrics", handleMetrics);
  server.on("/health", handleHealth);
  
  // Advanced AI
  server.on("/agents", handleAgents);
  server.on("/rag", handleRAG);
  server.on("/chain-of-thought", handleChainOfThought);
  server.on("/embeddings", handleEmbeddings);
  server.on("/tool-use", handleToolUse);
  server.on("/multimodal", handleMultimodal);
  
  // Safety & Tuning
  server.on("/guardrails", handleGuardrails);
  server.on("/fine-tune", handleFineTune);
  
  // OpenAI-Compatible API
  server.on("/v1/chat/completions", HTTP_POST, handleChatCompletions);
  server.on("/v1/models", HTTP_GET, handleModels);
  
  server.begin();
  startTime = millis();
  
  int totalVocab = NUM_NOUNS + NUM_VERBS + NUM_ADJ + NUM_FILLERS;
  Serial.println("Model loaded. Ready for inference.");
  Serial.print("Vocabulary: ");
  Serial.print(totalVocab);
  Serial.println(" tokens");
  Serial.print("Agents available: ");
  Serial.println(NUM_AGENTS);
  Serial.println("Endpoints: 16");
  Serial.println("Accepting requests on port 80.");
}

void loop() {
  server.handleClient();
}
