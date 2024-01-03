//package edu.stevens.cs548.clinic.test;
//
//import java.text.SimpleDateFormat;
//import java.util.logging.Logger;
//
//import javax.annotation.PostConstruct;
//import javax.ejb.LocalBean;
//import javax.ejb.Singleton;
//import javax.ejb.Startup;
//import javax.persistence.EntityManager;
//import javax.persistence.PersistenceContext;
//
//import edu.stevens.cs548.clinic.domain.ClinicGatewayFactory;
//import edu.stevens.cs548.clinic.domain.IClinicGateway;
//import edu.stevens.cs548.clinic.domain.IPatientDAO;
//import edu.stevens.cs548.clinic.domain.IPatientFactory;
//import edu.stevens.cs548.clinic.domain.IProviderDAO;
//import edu.stevens.cs548.clinic.domain.IProviderFactory;
//import edu.stevens.cs548.clinic.domain.Patient;
//import edu.stevens.cs548.clinic.domain.Provider;
//import edu.stevens.cs548.clinic.domain.ProviderSpecEnum;
///**
// * Session Bean implementation class TestBean
// */
//@Singleton
//@LocalBean
//@Startup
//public class TestBean {
//  
//  private static Logger logger = Logger.getLogger(TestBean.class.getCanonicalName());
//
//  private static void info(String m) {
//      logger.info(m);
//  }
//  
//    public TestBean() { }
//    
//    @PersistenceContext(unitName = "ClinicDomain")
//  private EntityManager em;
//    
//    @PostConstruct
//    public void init() {
//      try {
//      info("-----------begin-------------");
//      info("Lingfei Hu");
//      
//      SimpleDateFormat sdf = new SimpleDateFormat( "yyyy-MM-dd");
//      
//      IClinicGateway clinicGateway = ClinicGatewayFactory.createClinicGateway();
//      IPatientFactory patientFactory = clinicGateway.getPatientFactory();
//      IPatientDAO patientDAO = clinicGateway.getPatientDAO();
//      IProviderFactory providerFactory = clinicGateway.getProviderFactory();
//      IProviderDAO providerDAO = clinicGateway.getProviderDAO();
//      
//      // add several patients
//      Patient newPatient = patientFactory.createPatient(357278, "Brown", sdf.parse("1965-05-06"));
//      long id = patientDAO.addPatient(newPatient.getName(), newPatient.getBirthDate(), 49, newPatient.getPatientId());
//      info("Patient: id = " + id + ", pid = " + newPatient.getPatientId() + " has been inserted into DB.");
//      
//      
//      newPatient = patientFactory.createPatient(848032, "Jones", sdf.parse("1988-03-18"));
//      id = patientDAO.addPatient(newPatient.getName(), newPatient.getBirthDate(), 26, newPatient.getPatientId());
//      info("Patient: id = " + id + ", pid = " + newPatient.getPatientId() + " has been inserted into DB.");
//      
//      newPatient = patientFactory.createPatient(367267, "Davis", sdf.parse("1996-01-25"));
//      id = patientDAO.addPatient(newPatient.getName(), newPatient.getBirthDate(), 18, newPatient.getPatientId());
//      info("Patient: id = " + id + ", pid = " + newPatient.getPatientId() + " has been inserted into DB.");
//      
//      
//      // add several providers
//      Provider newProvider = providerFactory.createProvider(694594, "Smith", ProviderSpecEnum.Radiology);
//      providerDAO.addProvider(newProvider.getNPI(), newProvider.getName(), newProvider.getSpecialization());
//      info("Provider: NPI = " + newProvider.getNPI() + " has been inserted into DB.");
//      
//      newProvider = providerFactory.createProvider(576834, "Johnson", ProviderSpecEnum.Surgery);
//      providerDAO.addProvider(newProvider.getNPI(), newProvider.getName(), newProvider.getSpecialization());
//      info("Provider: NPI = " + newProvider.getNPI() + " has been inserted into DB.");
//      
//      newProvider = providerFactory.createProvider(936767, "Williams", ProviderSpecEnum.Oncology);
//      providerDAO.addProvider(newProvider.getNPI(), newProvider.getName(), newProvider.getSpecialization());
//      info("Provider: NPI = " + newProvider.getNPI() + " has been inserted into DB.");
//      
//      // add several treatments
//      Patient patient1 = patientDAO.getPatientByPatientId(357278);
//      Provider provider1 = providerDAO.getProviderByNPI(576834);
//      long drugTreatmentId = patient1.addDrugTreatment("code", "cold medication", 18.0f, provider1);
//      info("drug treatment: " + drugTreatmentId + " has been inserted into DB.");
//      long surgeryTreatmentId = provider1.addSurgeryTreatment("Appendicitis", sdf.parse("1996-04-25"), patient1);
//      info("surgery treatment: " + surgeryTreatmentId + " has been inserted into DB.");
//      
//      info("The count of treatments associated with patientId " + patient1.getPatientId() + ": " + patient1.getTreatmentIds().size());
//      info("The count of treatments associated with NPI " + provider1.getNPI() + ": " + provider1.getTreatments().size());
//      
//      patient1.deleteTreatment(drugTreatmentId);
//      info("drug treatment: " + drugTreatmentId + " has been deleted from DB.");
//      info("The count of treatments associated with patientId " + patient1.getPatientId() + ": " + patient1.getTreatmentIds().size());
//      info("The count of treatments associated with NPI " + provider1.getNPI() + ": " + provider1.getTreatments().size());
//      
//      provider1.deleteTreatment(surgeryTreatmentId);
//      info("surgery treatment: " + surgeryTreatmentId + " has been deleted from DB.");
//      info("The count of treatments associated with patientId " + patient1.getPatientId() + ": " + patient1.getTreatmentIds().size());
//      info("The count of treatments associated with NPI " + provider1.getNPI() + ": " + provider1.getTreatments().size());
//      
//      info("-----------end-------------");
//      }
//      catch(Exception ex) {
//          info(ex.getMessage());
////            StackTraceElement[] elements = ex.getStackTrace();
////            for(StackTraceElement e: elements) {
////                info(e.toString());
////            }
//      }
//      
//      
//    }
//    
//}
